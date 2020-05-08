#include <iostream>
#include <SDL.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <SDL_mixer.h>
#include <cstring>
#include <SDL_image.h>

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
    Mix_Music* ketbai = Mix_LoadMUS("ketbai.mp3");
    Mix_PlayMusic(ketbai, 1);
    SDL_Delay(3000);
    Mix_CloseAudio();
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
    food.x = (rand() % ((SCREEN_WIDTH - 20)/20))*20;
    food.y = (rand() % ((SCREEN_HEIGHT - 20)/20))*20;
    food.w = 20;
    food.h = 20;
}
bool update(vector <SDL_Rect>& snake, const int td, const int dd) {
    for (int i = snake.size() - 1; i > 0; i--) {
        if (td == 1 && snake[i].x == dd && snake[i].y == snake[0].y) return false;
        if (td == 2 && snake[i].y == dd && snake[i].x == snake[0].x) return false;
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
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
    {
        cout << "Error";
    }
    Mix_Music* music = NULL;
    Mix_Music* modau = NULL;
    Mix_Music* ketngan = NULL;
    ketngan = Mix_LoadMUS("ketngan.mp3");
    modau = Mix_LoadMUS("modau.mp3");
    music = Mix_LoadMUS("music.mp3");
    Mix_PlayMusic(modau, 1);
    hcn.x = 760;
    hcn.y = 380;
    hcn.w = 20;
    hcn.h = 20;
    snake.push_back(hcn);
    hcn.x = 740;
    hcn.y = 380;
    hcn.w = 20;
    hcn.h = 20;
    snake.push_back(hcn);
    const int step = 20;
    SDL_RenderFillRect(renderer, &hcn);
    refreshScreen( renderer, snake, food);
    // Your drawing code here
    // use SDL_RenderPresent(renderer) to show it
    SDL_Event e;
    int nho = 2, dem = 0;
    SDL_WaitEvent(&e);
    while (true) {
        
        // Nếu không có sự kiện gì thì tiếp tục trở về đầu vòng lặp
        
                    // Nếu sự kiện là kết thúc (như đóng cửa sổ) thì thoát khỏi vòng lặp
        //auto(snake, nho);
        if (SDL_PollEvent(&e) == 0) {
            SDL_Delay(100);
            switch (nho) {
            case 1:
                if (update(snake, 1, (snake[0].x - step + SCREEN_WIDTH) % SCREEN_WIDTH));
                else {
                    quitSDL(window, renderer);
                    return 0;
                }
                snake[0].x = (snake[0].x - step + SCREEN_WIDTH) % SCREEN_WIDTH;
                break;
            case 2:
                if (update(snake, 1, (snake[0].x + step) % SCREEN_WIDTH));
                else {
                    quitSDL(window, renderer);
                    return 0;
                }
                snake[0].x = (snake[0].x + step + SCREEN_WIDTH) % SCREEN_WIDTH;
                break;
            case 3:
                if (update(snake, 2, (snake[0].y + step + SCREEN_HEIGHT) % SCREEN_HEIGHT));
                else {
                    quitSDL(window, renderer);
                    return 0;
                }
                snake[0].y = (snake[0].y + step + SCREEN_HEIGHT) % SCREEN_HEIGHT;
                break;
            case 4:
                if (update(snake, 2, (snake[0].y - step + SCREEN_HEIGHT) % SCREEN_HEIGHT));
                else {
                    quitSDL(window, renderer);
                    return 0;
                }
                snake[0].y = (snake[0].y - step + SCREEN_HEIGHT) % SCREEN_HEIGHT;
                break;
            default: break;
            }
            if (snake[0].x == food.x && snake[0].y == food.y) {
                snake.push_back(hcn);
                if (dem % 2 == 0) Mix_PlayMusic(music, 1);
                else Mix_PlayMusic(ketngan, 1);
                dem++;
            }
            while (snake[0].x == food.x && snake[0].y == food.y) randomfood(food);
            refreshScreen(renderer, snake, food);
            continue;
        }
        if (e.type == SDL_QUIT) break;
        // Nếu có một phím được nhấn, thì xét phím đó là gì để xử lý tiếp
        hcn = snake[snake.size() - 1];
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_ESCAPE: break; // Nếu nhấn phìm ESC thì thoát khỏi vòng lặp
            // Nếu phím mũi tên trái, dịch sang trái 
            // (cộng chiều rộng, trừ bước, rồi lấy phần dư để giá trị luôn dương, và hiệu ứng quay vòng)
            case SDLK_LEFT:
                if (update(snake, 1, (snake[0].x + SCREEN_WIDTH - step) % SCREEN_WIDTH));
                else {
                    quitSDL(window, renderer);
                    return 0;
                }
                nho = 1;
                snake[0].x = (snake[0].x + SCREEN_WIDTH - step) % SCREEN_WIDTH;
                break;
                // Tương tự với dịch phải, xuống và lên
            case SDLK_RIGHT:
                if (update(snake, 1, (snake[0].x + step) % SCREEN_WIDTH));
                else {
                    quitSDL(window, renderer);
                    return 0;
                }
                nho = 2;
                snake[0].x = (snake[0].x + step) % SCREEN_WIDTH;
                break;
            case SDLK_DOWN:
                if (update(snake, 2, (snake[0].y + step) % SCREEN_HEIGHT));
                else {
                    quitSDL(window, renderer);
                    return 0;
                }
                nho = 3;
                snake[0].y = (snake[0].y + step) % SCREEN_HEIGHT;
                break;
            case SDLK_UP:
                if (update(snake, 2, (snake[0].y + SCREEN_HEIGHT - step) % SCREEN_HEIGHT));
                else {
                    quitSDL(window, renderer);
                    return 0;
                }
                nho = 4;
                snake[0].y = (snake[0].y + SCREEN_HEIGHT - step) % SCREEN_HEIGHT;
                break;
            default: break;
            }
            if (snake[0].x == food.x && snake[0].y == food.y) {
                snake.push_back(hcn);
                if (dem % 2 == 0) Mix_PlayMusic(music, 1);
                else Mix_PlayMusic(ketngan, 1);
                dem++;
            }
            while (snake[0].x == food.x && snake[0].y == food.y) randomfood(food);
            // Xoá toàn bộ màn hình và vẽ lại
            refreshScreen(renderer, snake, food);
        }
        
        
    }
    //waitUntilKeyPressed();
    Mix_CloseAudio();
    quitSDL(window, renderer);
    return 0;
}


