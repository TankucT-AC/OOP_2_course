#include "GameBoard.h"

GameBoard::GameBoard(int InitX, int InitY, int InitColWidth, int InitColHeight, Camera* InitCamera) : 
    Point(InitX, InitY), currentCamera(InitCamera), colWidth(InitColWidth), colHeight(InitColHeight), keysCollected(0)
{
    exitDoor.SetPosition(X + colWidth / 2, Y);
}

GameBoard::~GameBoard() {}

void GameBoard::Show(Camera* cam) {
    int radius = cam->GetRadius();
    int camX = cam->GetX();
    int camY = cam->GetY();

    // 1. Создаем регионы для "дырки" камеры
    HRGN hRectRgn = CreateRectRgn(X - colWidth / 2, Y - colHeight / 2, X + colWidth / 2, Y + colHeight / 2);
    HRGN hCircleRgn = CreateEllipticRgn(camX - radius, camY - radius, camX + radius, camY + radius);
    HRGN hDestRgn = CreateRectRgn(0, 0, 0, 0);

    CombineRgn(hDestRgn, hRectRgn, hCircleRgn, RGN_DIFF);

    // Рисуем радиус видимости камеры
    DrawCameraRim(cam);

    // 2. Рисуем черное поле
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    FillRgn(memDC, hDestRgn, blackBrush);

    // 3. Рисуем дверь
    // Если открыта — зеленый (RGB 0, 255, 0), если закрыта — красный (RGB 255, 0, 0)
    COLORREF doorColor = exitDoor.isOpen ? RGB(0, 255, 0) : RGB(255, 0, 0);
    HBRUSH doorBrush = CreateSolidBrush(doorColor);

    RECT doorRect = {
        exitDoor.dx - 5,
        exitDoor.dy - exitDoor.height / 2,
        exitDoor.dx + 5,
        exitDoor.dy + exitDoor.height / 2
    };
    FillRect(memDC, &doorRect, doorBrush);

    // 4. Внешняя рамка поля
    HPEN fieldPen = CreatePen(PS_SOLID, 3, RGB(128, 128, 128));
    SelectObject(memDC, fieldPen);
    SelectObject(memDC, GetStockObject(HOLLOW_BRUSH));
    Rectangle(memDC, X - colWidth / 2, Y - colHeight / 2, X + colWidth / 2, Y + colHeight / 2);

    int barWidth = 200;
    int barHeight = 20;
    int barX = X - colWidth / 2 + 20; // Позиция в верхнем левом углу поля
    int barY = Y - colHeight / 2 + 20;

    // 5. Эффект помех при низком заряде
    float chargePct = battery.GetLevel();
    if (chargePct < 25.0f) {
        // Рассчитываем интенсивность шума (чем меньше заряд, тем больше точек)
        // При 25% — мало, при 1% — очень много
        int noiseIntensity = (int)(30 - chargePct);

        int camX = cam->GetX();
        int camY = cam->GetY();
        int radius = cam->GetRadius();

        for (int i = 0; i < 500; i++) {
            // Генерируем случайную точку внутри квадрата камеры
            int dx = (rand() % (radius * 2)) - radius;
            int dy = (rand() % (radius * 2)) - radius;

            // Проверяем, попадает ли точка в круг (чтобы шум не выходил за пределы дырки)
            if (dx * dx + dy * dy <= radius * radius) {
                // Выбираем цвет шума: белый, серый или темно-серый
                COLORREF noiseColor = (rand() % 2 == 0) ? RGB(255, 255, 255) : RGB(100, 100, 100);
                SetPixel(memDC, camX + dx, camY + dy, noiseColor);
            }
        }
    }

    // Линия помехи
    if (chargePct < 15.0f && (rand() % 5 == 0)) 
    {
        int camX = cam->GetX();
        int camY = cam->GetY();
        int radius = cam->GetRadius();

        int lineY = camY + (rand() % (radius * 2)) - radius;
        HPEN noisePen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
        SelectObject(memDC, noisePen);

        MoveToEx(memDC, camX - radius + 10, lineY, NULL);
        LineTo(memDC, camX + radius - 10, lineY);

        DeleteObject(noisePen);
    }

    // 6. Рамка шкалы заряда
    RECT frameRect = { barX, barY, barX + barWidth, barY + barHeight };
    FrameRect(memDC, &frameRect, (HBRUSH)GetStockObject(GRAY_BRUSH));

    // Заполнение шкалы
    float currentLevel = battery.GetLevel();
    int fillWidth = (int)((currentLevel / 100.0f) * barWidth);

    RECT fillRect = { barX + 1, barY + 1, barX + fillWidth - 1, barY + barHeight - 1 };
    HBRUSH batteryBrush = CreateSolidBrush(battery.GetColor());
    FillRect(memDC, &fillRect, batteryBrush);

    DeleteObject(batteryBrush);

    // Вывод текста если батерея села
    if (battery.IsEmpty()) 
    {
        SetTextColor(memDC, RGB(255, 0, 0));
        TextOutA(memDC, barX, barY + 25, "BATTERY CRITICAL", 16);
    }

    // Позиция для начала отрисовки инвентаря (под батареей)
    int invX = X - colWidth / 2 + 20;
    int invY = Y - colHeight / 2 - 40;

    // Надпись "KEYS:"
    SetTextColor(memDC, RGB(255, 255, 255));
    SetBkMode(memDC, TRANSPARENT);
    TextOutA(memDC, invX, invY, "KEYS:", 5);

    // Рисуем иконки ключей в ряд
    for (int i = 0; i < keysCollected; i++) {
        // Каждый следующий ключ рисуем правее предыдущего
        DrawKeyIcon(invX + 60 + (i * 40), invY - 5, 0.8f);
    }

    // Если собрано 5 ключей, выведем подсказку
    if (keysCollected >= 5) {
        SetTextColor(memDC, RGB(0, 255, 0));
        TextOutA(memDC, invX, invY + 20, "EXIT OPEN!", 10);
    }

    // Очистка
    DeleteObject(hRectRgn);
    DeleteObject(hCircleRgn);
    DeleteObject(hDestRgn);
    DeleteObject(blackBrush);
    DeleteObject(doorBrush);
    DeleteObject(fieldPen);
}

void GameBoard::GameOver()
{
    HFONT hFont = CreateFontA(
        100, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        VARIABLE_PITCH, "Arial"
    );

    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 0, 0));

    // Текст будет выровнен по центру прямоугольника поля
    RECT textRect = { X - colWidth / 2, Y - colHeight / 2, X + colWidth / 2, Y + colHeight / 2 };

    DrawTextA(hdc, "GAME OVER", -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // Очистка ресурсов
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}

void GameBoard::YouWin()
{
    HFONT hFont = CreateFontA(
        100, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        VARIABLE_PITCH, "Arial"
    );

    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(0, 255, 0));

    RECT textRect = { X - colWidth / 2, Y - colHeight / 2, X + colWidth / 2, Y + colHeight / 2 };

    DrawTextA(hdc, "YOU WIN!", -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}

void GameBoard::DrawKeyIcon(int x, int y, float scale)
{
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 215, 0));
    SelectObject(memDC, hBrush);

    // Головка
    Ellipse(memDC, x, y, x + (int)(20 * scale), y + (int)(20 * scale));
    // Стержень
    Rectangle(memDC, x + (int)(20 * scale), y + (int)(8 * scale),
        x + (int)(50 * scale), y + (int)(12 * scale));
    // Зубцы
    Rectangle(memDC, x + (int)(40 * scale), y + (int)(12 * scale),
        x + (int)(45 * scale), y + (int)(18 * scale));

    DeleteObject(hBrush);
}

void GameBoard::DrawCameraRim(Camera* cam)
{
    int r = cam->GetRadius();
    int cx = cam->GetX();
    int cy = cam->GetY();

    // 1. Создаем перо для ободка
    // Можно сделать его светло-серым или слегка светящимся
    HPEN rimPen = CreatePen(PS_SOLID, 6, RGB(80, 80, 80));
    HGDIOBJ oldPen = SelectObject(memDC, rimPen);

    // Используем пустую кисть, чтобы не закрасить внутренности дырки
    SelectObject(memDC, GetStockObject(HOLLOW_BRUSH));

    // 2. Рисуем окружность
    // Важно: рисуем чуть-чуть шире или по самому краю радиуса
    Ellipse(memDC, cx - r, cy - r, cx + r, cy + r);

    // 3. Чистим за собой
    SelectObject(memDC, oldPen);
    DeleteObject(rimPen);
}