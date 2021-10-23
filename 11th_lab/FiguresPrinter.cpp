#include "FiguresPrinter.h"

#include "PaintTools.h"
#include "IDCModifier.h"
#include "IDCPrinter.h"
#include "DCEllipsePrinter.h"
#include "DCRectanglePrinter.h"
#include "DCRhombusPrinter.h"
#include "DCStarPrinter.h"

#include <random>

FiguresPrinter::FiguresPrinter(
  std::shared_ptr<ModifiableBoard> board
)
  : __board(std::move(board))
{}
FiguresPrinter::~FiguresPrinter()
{
  Stop();
}

DWORD WINAPI ThreadFunction( LPVOID lpParam );

void FiguresPrinter::Run()
{
  if (__isRunning) {
    return; 
  }
  __threadDataArray = new ThreadData[THREAD_COUNT];
  for (int i = 0; i < THREAD_COUNT; ++i) {
    __threadDataArray[i].board = __board;
  }

  for (int i = 0; i < THREAD_COUNT; ++i)
  {
    auto& handler = __threadArray[i];
    auto& data = __threadDataArray[i];
    auto& id = __threadIdArray[i];

    handler = CreateThread( 
      NULL,
      0,
      ThreadFunction,
      &data,
      0,
      &id);

    if (handler == NULL) {
      ExitProcess(3);
    }
  } 

  __isRunning = true;
}

void FiguresPrinter::Stop()
{
  if (!__isRunning) {
    return; 
  }

  for (int i = 0; i < THREAD_COUNT; ++i) {
    __threadDataArray[i].stop.test_and_set();
  }

  WaitForMultipleObjects(__threadArray.size(), 
                         __threadArray.data(), 
                         TRUE, 
                         INFINITE);
  for (auto& handle : __threadArray) {
    CloseHandle(handle);
  }

  delete[] __threadDataArray;

  __isRunning = false;
}

enum class Figure
{
  circle, 
  rectangle, 
  rhombus, 
  star, 
  END
};

DCPrinter GetFigure(Figure fig);
int GenerateRandom(int from, int to);

DWORD WINAPI ThreadFunction(LPVOID lpParam) 
{ 
  auto& data = *(FiguresPrinter::ThreadData*)lpParam;

  while (!data.stop.test()) {
    Sleep(GenerateRandom(10, 200));
    auto [width, height] = data.board->GetSize();
    auto maxFigSize = min(width, height) / 3;
    auto minFigSize = maxFigSize / 4;

    Position figurePosition;
    figurePosition.w = GenerateRandom(minFigSize, maxFigSize);
    figurePosition.h = figurePosition.w;
    figurePosition.x = GenerateRandom(0, width - figurePosition.w);
    figurePosition.y = GenerateRandom(0, height - figurePosition.h);

    COLORREF brushColor = RGB(
      GenerateRandom(0, 255), 
      GenerateRandom(0, 255), 
      GenerateRandom(0, 255)
    );

    COLORREF penColor = BLACK;

    Figure fig = static_cast<Figure>(
      GenerateRandom(0, static_cast<int>(Figure::END))
    );

    DCPrinter modifier = GetFigure(fig);
    if (!modifier) {
      continue;
    }
    modifier->SetBeginPoint(
      POINT{
        figurePosition.x, 
        figurePosition.y
      }
    );
    modifier->SetEndPoint(
      POINT{
        figurePosition.x + figurePosition.w, 
        figurePosition.y + figurePosition.h
      }
    );
    modifier->SetBrushColor(brushColor);
    modifier->SetPenColor(penColor);

    data.board->AddModifier(modifier);
    Sleep(GenerateRandom(100, 600));
    data.board->RemoveModifier(modifier);
  }

  return 0; 
} 

DCPrinter GetFigure(Figure fig)
{
  switch (fig) {
    case Figure::circle:
      return std::make_unique<DCEllipsePrinter>();
    case Figure::rectangle:
      return std::make_unique<DCRectanglePrinter>();
    case Figure::rhombus:
      return std::make_unique<DCRhombusPrinter>();
    case Figure::star:
      return std::make_unique<DCStarPrinter>();
    default:
      return nullptr;
  }
}

int GenerateRandom(int from, int to)
{
  std::random_device r;
  std::default_random_engine e1(r());
  return std::uniform_int_distribution<int>(from, to)(e1);
}
