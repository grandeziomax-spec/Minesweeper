#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>
sf::Vector2i getTexturePosition(int Val)
{
    if (Val >= 1 && Val <= 4)
    {
        return { (Val - 1) * 16, 0 };
    }
    if (Val >= 5 && Val <= 8)
    {
        return { (Val - 5) * 16, 16 };
    }
    if (Val == 0)
    {
        return { 0, 32 };  // Пустая
    }
    if (Val == 10)
    {
        return { 16, 32 }; // Закрытая
    }
    if (Val == 9)
    {
        return { 32, 32 }; // Мина
    }
    if (Val == 11)
    {
        return { 48, 32 }; // Красный флаг
    }

    return { 16, 32 }; //возвращаем закрытую
}
int main()
{
    srand(static_cast<unsigned int>(time(0)));
    int w = 64;
    sf::RenderWindow app(sf::VideoMode({ 768, 768 }), "Minesweeper SFML 3.1");
    app.setFramerateLimit(60);
    sf::Texture t;
    if (!t.loadFromFile("images/tiles.jpg"))
    {
        std::cerr << "net kartinki!" << std::endl;
        return -1;
    }
    sf::Sprite s(t);
    int grid[12][12] = { 0 };
    int sgrid[12][12] = { 0 };
    for (int i = 1; i <= 10; i++)
    {
        for (int j = 1; j <= 10; j++)
        {
            sgrid[i][j] = 10; //все клетки закрыты
            if (rand() % 5 == 0)
            {
                grid[i][j] = 9;
            }
            else
            {
                grid[i][j] = 0;
            }
        }
    }
    for (int i = 1; i <= 10; i++)
    {
        for (int j = 1; j <= 10; j++)
        {
            if (grid[i][j] == 9) continue;
            int mineCount = 0;
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    if (grid[i + di][j + dj] == 9)
                    {
                        mineCount++;
                    }
                }
            }
            grid[i][j] = mineCount;
        }
    }
    while (app.isOpen())
    {
        while (const std::optional event = app.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) //нажатие на крестик
            {
                app.close();
            }
            if (const auto* click = event->getIf<sf::Event::MouseButtonPressed>())
            {
                int j = click->position.x / w;
                int i = click->position.y / w;
                if (i < 1 || i > 10 || j < 1 || j > 10) continue;
                if (click->button == sf::Mouse::Button::Left)
                {
                    if (sgrid[i][j] == 10)
                    {
                        sgrid[i][j] = grid[i][j];
                        if (grid[i][j] == 9)
                        {
                            for (int r = 1; r <= 10; r++)
                            {
                                for (int c = 1; c <= 10; c++)
                                {
                                    if (grid[r][c] == 9)
                                    {
                                        sgrid[r][c] = 9;
                                    }
                                }
                            }
                        }
                    }
                }
                else if (click->button == sf::Mouse::Button::Right)
                {
                    if (sgrid[i][j] == 10)
                    {
                        sgrid[i][j] = 11; // ставим флаг
                    }
                    else if (sgrid[i][j] == 11)
                    {
                        sgrid[i][j] = 10; // снимаем флаг
                    }
                }
            }
        }
        app.clear(sf::Color(180, 180, 180));
        for (int i = 1; i <= 10; i++)
        {
            for (int j = 1; j <= 10; j++)
            {
                s.setTextureRect({ getTexturePosition(sgrid[i][j]), { 16, 16 } });
                s.setScale({ 4.0f, 4.f });
                s.setPosition({ static_cast<float>(j * w), static_cast<float>(i * w) });
                app.draw(s);
            }
        }
        app.display();
    }
    return 0;
}
