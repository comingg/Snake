#include <iostream>
#include <SDL.h>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

const int SCREEN_WIDTH = 1520;
const int SCREEN_HEIGHT = 760;
const string WINDOW_TITLE = "Snakerman";

void logSDLError(std::ostream& os,
    const std::string& msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window*& window, SDL_Renderer*& renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);


    //Khi thông thường chạy với môi trường bình thường ở nhà
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
        SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if (SDL_WaitEvent(&e) != 0 &&
            (e.type == SDL_KEYDOWN || e.type == SDL_QUIT))
            return;
        SDL_Delay(200);
    }
}
void refreshScreen( SDL_Renderer* renderer, const vector <SDL_Rect> snake,const SDL_Rect &food)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &snake[0]);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    for (int i = 1; i < snake.size(); i++)
    {
        SDL_RenderFillRect(renderer, &snake[i]);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &food);
    SDL_RenderPresent(renderer);
}
void randomfood(SDL_Rect & food) {
    food.x = (rand() % ((SCREEN_WIDTH - 40)/40))*40;
    food.y = (rand() % ((SCREEN_HEIGHT - 40)/40))*40;
    food.w = 40;
    food.h = 40;
}
bool update(vector <SDL_Rect>& snake, const int td, const int x) {
    for (int i = snake.size() - 1; i > 0; i--) {
        if (td == 1 && snake[i].x == x && snake[i].y == snake[0].y) return false;
        if (td == 2 && snake[i].y == x && snake[i].x == snake[0].x) return false;
        snake[i] = snake[i - 1];
    }
    return true;
}
int main(int argc, char* argv[])
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
    srand(time(NULL));
    SDL_Rect food,hcn;
    randomfood(food);
    vector <SDL_Rect> snake;
    hcn.x = 0;
    hcn.y = 0;
    hcn.w = 40;
    hcn.h = 40;
    snake.push_back(hcn);
    const int step = 40;
    SDL_RenderFillRect(renderer, &hcn);
    refreshScreen( renderer, snake, food);
    // Your drawing code here
    // use SDL_RenderPresent(renderer) to show it
    SDL_Event e;
    while (true) {
        //SDL_Delay(0);

        // Nếu không có sự kiện gì thì tiếp tục trở về đầu vòng lặp
        if (SDL_WaitEvent(&e) == 0) continue;

        // Nếu sự kiện là kết thúc (như đóng cửa sổ) thì thoát khỏi vòng lặp
        if (e.type == SDL_QUIT) break;
        // Nếu có một phím được nhấn, thì xét phím đó là gì để xử lý tiếp
        hcn = snake[snake.size() - 1];
        cout << snake.size() << endl;
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_ESCAPE: break; // Nếu nhấn phìm ESC thì thoát khỏi vòng lặp
            // Nếu phím mũi tên trái, dịch sang trái 
            // (cộng chiều rộng, trừ bước, rồi lấy phần dư để giá trị luôn dương, và hiệu ứng quay vòng)
            case SDLK_LEFT: 
                if (update(snake,1, (snake[0].x + SCREEN_WIDTH - step) % SCREEN_WIDTH));
                else {
                    quitSDL(window, renderer);
                    return 0;
                }
                snake[0].x = (snake[0].x + SCREEN_WIDTH - step) % SCREEN_WIDTH;
                break;
                // Tương tự với dịch phải, xuống và lên
            case SDLK_RIGHT: 
                if (update(snake,1, (snake[0].x + step) % SCREEN_WIDTH));
                else {
                    quitSDL(window, renderer);
                    return 0;
                }
                snake[0].x = (snake[0].x + step) % SCREEN_WIDTH; 
                break;
            case SDLK_DOWN:
                if (update(snake,2, (snake[0].y + step) % SCREEN_HEIGHT));
                else {
                    quitSDL(window, renderer);
                    return 0;
                }
                snake[0].y = (snake[0].y + step) % SCREEN_HEIGHT; 
                break;
            case SDLK_UP: 
                if (update(snake,2, (snake[0].y + SCREEN_HEIGHT - step) % SCREEN_HEIGHT));
                else {
                    quitSDL(window, renderer);
                    return 0;
                }
                snake[0].y = (snake[0].y + SCREEN_HEIGHT - step) % SCREEN_HEIGHT; 
                break;
            default: break;
            }
            if (snake[0].x == food.x && snake[0].y == food.y) snake.push_back(hcn);
            while (snake[0].x==food.x && snake[0].y==food.y) randomfood(food);
            // Xoá toàn bộ màn hình và vẽ lại
            refreshScreen( renderer, snake ,food);

        }
    }
    //waitUntilKeyPressed();
    quitSDL(window, renderer);
    return 0;
}


