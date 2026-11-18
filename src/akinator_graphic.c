#include <akinator_graphic.h>

akinator_screens_t akinator_screen = FIRST_SCREEN;
char string[100] = {};
char buffer[256] = {};
int letters = 0;

int main()
{
    SetTraceLogLevel(LOG_NONE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Akinator");
    SetTargetFPS(120);

    leaf_t* root = NULL;
    leaf_t* current = root;

    button_t MenuButtons[] =
    {
        [QUIT] = {"QUIT", {SCREEN_WIDTH * 0.7, SCREEN_HEIGHT * 0.75, SCREEN_WIDTH * 0.15, 60}, MENU_BUTTONS_FONT_SIZE, LIGHTGRAY, GRAY},
        [PLAY] = {"PLAY", {SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.75, SCREEN_WIDTH * 0.15, 60}, MENU_BUTTONS_FONT_SIZE, LIGHTGRAY, GRAY},
        [SAVE] = {"SAVE", {SCREEN_WIDTH * 0.3, SCREEN_HEIGHT * 0.75, SCREEN_WIDTH * 0.15, 60}, MENU_BUTTONS_FONT_SIZE, LIGHTGRAY, GRAY},
        [LOAD] = {"LOAD", {SCREEN_WIDTH * 0.1, SCREEN_HEIGHT * 0.75, SCREEN_WIDTH * 0.15, 60}, MENU_BUTTONS_FONT_SIZE, LIGHTGRAY, GRAY}
    };

    button_t AkinatorScreenButtons[] =
    {
        [NEXT] = {"NEXT", {SCREEN_WIDTH * 0.8,           SCREEN_HEIGHT * 0.6, SCREEN_WIDTH * 0.15, 60}, AKINATOR_BUTTONS_FONT_SIZE, LIGHTGRAY, GRAY},
        [YES]  = {"YES",  {SCREEN_WIDTH * 0.2,           SCREEN_HEIGHT * 0.6, SCREEN_WIDTH * 0.25, SCREEN_HEIGHT * 0.25}, YES_NO_FONT_SIZE, GREEN, DARKGREEN},
        [NO]  =  {"NO",   {(float)(SCREEN_WIDTH * 0.55), SCREEN_HEIGHT * 0.6, SCREEN_WIDTH * 0.25, SCREEN_HEIGHT * 0.25}, YES_NO_FONT_SIZE, ORANGE, RED},
    };

    button_t SLScreenButtons[] =
    {
        [SAVE_BUTTON] = {"SAVE", {SCREEN_WIDTH * 0.8, SCREEN_HEIGHT * 0.4, SCREEN_WIDTH * 0.15, 50}, 40, LIGHTGRAY, GRAY},
        [LOAD_BUTTON] = {"LOAD", {SCREEN_WIDTH * 0.8, SCREEN_HEIGHT * 0.4, SCREEN_WIDTH * 0.15, 50}, 40, LIGHTGRAY, GRAY},
        [BACK_BUTTON] = {"BACK", {SCREEN_WIDTH * 0.35, SCREEN_HEIGHT * 0.6, SCREEN_WIDTH * 0.30, 60}, 60, LIGHTGRAY, GRAY},

    };

    SetButtonsTextPosition(MenuButtons, ARRAY_SIZE(MenuButtons));
    SetButtonsTextPosition(AkinatorScreenButtons, ARRAY_SIZE(AkinatorScreenButtons));
    SetButtonsTextPosition(SLScreenButtons, ARRAY_SIZE(SLScreenButtons));

    screen_status current_screen = SCREEN_MENU;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        switch (current_screen)
        {
            case SCREEN_MENU:
                switch (DrawMenuScreen(MenuButtons, ARRAY_SIZE(MenuButtons)))
                {
                    case QUIT:
                        if (root != NULL)
                            DeleteLeaf(root);
                        return 0;
                    case PLAY:
                        AkinatorScreenButtons[NEXT] = {"NEXT", {SCREEN_WIDTH * 0.8, SCREEN_HEIGHT * 0.6, SCREEN_WIDTH * 0.15, 60}, AKINATOR_BUTTONS_FONT_SIZE, LIGHTGRAY, GRAY};
                        SetButtonsTextPosition(AkinatorScreenButtons, ARRAY_SIZE(AkinatorScreenButtons));
                        current_screen = SCREEN_AKINATOR;
                        break;
                    case SAVE:
                        current_screen = SCREEN_SAVE;
                        break;
                    case LOAD:
                        current_screen = SCREEN_LOAD;
                        break;
                    default:
                        break;
                }
                break;
            case SCREEN_SAVE:
            {
                int button_status = DrawSLScreen(current_screen, SLScreenButtons, ARRAY_SIZE(SLScreenButtons));

                if (button_status == SAVE_BUTTON)
                {
                    strcpy(buffer, "databases/");
                    strcat(buffer, string);
                    SaveTree(root, buffer);
                    current_screen = SCREEN_MENU;
                }
                else if (button_status == BACK_BUTTON)
                    current_screen = SCREEN_MENU;
                else
                    break;

                memset(string, 0, 100);
                letters = 0;

                break;
            }
            case SCREEN_LOAD:
            {
                int button_status = DrawSLScreen(current_screen, SLScreenButtons, ARRAY_SIZE(SLScreenButtons));
                if (button_status == LOAD_BUTTON)
                {
                    strcpy(buffer, "databases/");
                    strcat(buffer, string);
                    root = LoadTree(buffer);
                    current = root;
                    current_screen = SCREEN_MENU;
                }
                else if (button_status == BACK_BUTTON)
                    current_screen = SCREEN_MENU;
                else
                    break;

                memset(string, 0, 100);
                letters = 0;

                break;
            }
            case SCREEN_AKINATOR:
                if (akinator_screen == LAST_QUESTION_SCREEN_NO1 || akinator_screen == LAST_QUESTION_SCREEN_NO2 || akinator_screen == LAST_QUESTION_SCREEN_YES)
                    ;
                else if (root == NULL)
                {
                    ON_LOG(LOG("akinator_screen = ADD_FIRST_ELEMENT"))
                    akinator_screen = ADD_FIRST_ELEMENT;
                }
                else
                {
                    if (current->left == NULL || current->right == NULL)
                    {
                        ON_LOG(LOG("akinator_screen = LAST_QUESTION_SCREEN"))
                        akinator_screen = LAST_QUESTION_SCREEN;
                    }
                    else
                    {
                        ON_LOG(LOG("akinator_screen = QUESTION_SCREEN"))
                        akinator_screen = QUESTION_SCREEN;
                    }

                }
                switch (DrawAkinatorGameScreen(current, AkinatorScreenButtons, ARRAY_SIZE(AkinatorScreenButtons)))
                {
                    case NEXT:
                        switch (akinator_screen)
                        {
                            case ADD_FIRST_ELEMENT:
                                root = InitLeaf(string);
                                ON_LOG(LOG("ADD_FIRST_ELEMENT: %s", string))
                                current = root;
                                memset(string, 0, 100);
                                letters = 0;
                                ON_LOG(LOG("ADD_FIRST_ELEMENT, after memset: string: %s, letters: %d", string, letters))
                                break;
                            case LAST_QUESTION_SCREEN_YES:
                                current_screen = SCREEN_MENU;
                                akinator_screen = ADD_FIRST_ELEMENT;
                                current = root;
                                break;
                            case LAST_QUESTION_SCREEN_NO1:
                                current->right = InitLeaf(current->data);
                                current->left = InitLeaf(string);
                                memset(string, 0, 100);
                                letters = 0;
                                akinator_screen = LAST_QUESTION_SCREEN_NO2;
                                break;
                            case LAST_QUESTION_SCREEN_NO2:
                                ChangeLeafData(current, string);
                                memset(string, 0, 100);
                                letters = 0;
                                current_screen = SCREEN_MENU;
                                akinator_screen = ADD_FIRST_ELEMENT;
                                current = root;
                                break;
                            default:
                                break;
                        }
                        break;
                    case YES:
                        if (akinator_screen == LAST_QUESTION_SCREEN)
                        {
                            akinator_screen = LAST_QUESTION_SCREEN_YES;
                            AkinatorScreenButtons[NEXT] = {"NEXT", {SCREEN_WIDTH * 0.35, SCREEN_HEIGHT * 0.6, SCREEN_WIDTH * 0.30, 60}, AKINATOR_BUTTONS_FONT_SIZE, LIGHTGRAY, GRAY};
                            SetButtonsTextPosition(AkinatorScreenButtons, ARRAY_SIZE(AkinatorScreenButtons));
                        }
                        else
                            current = current->left;
                        break;
                    case NO:
                        if (akinator_screen == LAST_QUESTION_SCREEN)
                        {
                            akinator_screen = LAST_QUESTION_SCREEN_NO1;
                        }
                        else
                            current = current->right;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
        EndDrawing();
    }
    if (root != NULL)
        DeleteLeaf(root);
    ON_LOG(ENDLOG())

    return 0;
}

int DrawSLScreen( screen_status current_screen, button_t* buttons, size_t size )
{
    SLSetButtonsVisibility(current_screen, buttons);

    int status = CheckButtonsStatus(buttons, size);
    if (status != -1)
        return status;

    ClearBackground(RAYWHITE);
    DrawButtons(buttons, size);

    if (current_screen == SCREEN_LOAD)
        DrawTextBeautiful("Add filename to load database", SCREEN_HEIGHT * 0.3, 40, GRAY);
    else if (current_screen == SCREEN_SAVE)
        DrawTextBeautiful("Add filename to save database", SCREEN_HEIGHT * 0.3, 40, GRAY);
    DrawInputBar(SCREEN_HEIGHT * 0.4, 40, LIGHTGRAY);

    return -1;
}

void SLSetButtonsVisibility( screen_status current_screen, button_t* buttons )
{
    if (current_screen == SCREEN_LOAD)
    {
        buttons[SAVE_BUTTON].is_invis = true;
        buttons[LOAD_BUTTON].is_invis = false;
    }
    else if (current_screen == SCREEN_SAVE)
    {
        buttons[SAVE_BUTTON].is_invis = false;
        buttons[LOAD_BUTTON].is_invis = true;
    }
}

int DrawAkinatorGameScreen( leaf_t* current, button_t* buttons, size_t size )
{
    AkinatorSetButtonsVisibility(buttons);
    DrawButtons(buttons, size);

    int status = CheckButtonsStatus(buttons, size);
    if (status != -1)
        return status;

    ClearBackground(RAYWHITE);

    int text_font = 50;

    if (akinator_screen == ADD_FIRST_ELEMENT)
    {
        DrawTextBeautiful("Add first character: ", SCREEN_HEIGHT * 0.4, text_font, GRAY);
        DrawInputBar(SCREEN_HEIGHT * 0.6, text_font, LIGHTGRAY);
    }
    else if (akinator_screen == QUESTION_SCREEN)
    {
        sprintf(buffer, "Is your character %s?", current->data);
        DrawTextBeautiful(buffer, SCREEN_HEIGHT * 0.4, text_font, GRAY);
    }
    else if (akinator_screen == LAST_QUESTION_SCREEN)
    {
        sprintf(buffer, "Is it %s?", current->data);
        DrawTextBeautiful(buffer, SCREEN_HEIGHT * 0.4, text_font, GRAY);
    }
    else if (akinator_screen == LAST_QUESTION_SCREEN_YES)
        DrawTextBeautiful("HE-HE-HE, It was really easy", SCREEN_HEIGHT * 0.4, text_font, GRAY);
    else if (akinator_screen == LAST_QUESTION_SCREEN_NO1)
    {
        DrawTextBeautiful("HMMM, I don't know him/her, lets add him/her in my brain", SCREEN_HEIGHT * 0.3, text_font, GRAY);
        DrawTextBeautiful("What is his/her name?", SCREEN_HEIGHT * 0.4, text_font, GRAY);
        DrawInputBar(SCREEN_HEIGHT * 0.6, text_font, LIGHTGRAY);
    }
    else if (akinator_screen == LAST_QUESTION_SCREEN_NO2)
    {
        sprintf(buffer, "How is %s different from %s? He/she is ...", current->left->data, current->right->data);
        DrawTextBeautiful(buffer, SCREEN_HEIGHT * 0.4, text_font, GRAY);
        DrawInputBar(SCREEN_HEIGHT * 0.6, text_font, LIGHTGRAY);
    }

    return -1;
}



void DrawTextBeautiful(const char* text, int top_y, int text_font, Color color) //TODO - сделать по красоте
{
    while (MeasureText(text, text_font) > 0.8 * SCREEN_WIDTH )
        text_font -= 2;
    DrawText(text, (SCREEN_WIDTH - MeasureText(text, text_font)) / 2, top_y, text_font, color);
}

int DrawMenuScreen( button_t* buttons, size_t size )
{
    int status = CheckButtonsStatus(buttons, size);
    if (status != -1)
        return status;


    ClearBackground(RAYWHITE);

    DrawButtons(buttons, size);

    return -1;
}

void AkinatorSetButtonsVisibility( button_t* buttons )
{
    if (akinator_screen == QUESTION_SCREEN || akinator_screen == LAST_QUESTION_SCREEN)
    {
        buttons[NEXT].is_invis = true;
        buttons[YES].is_invis = false;
        buttons[NO].is_invis = false;
    }
    else
    {
        buttons[NEXT].is_invis = false;
        buttons[YES].is_invis = true;
        buttons[NO].is_invis = true;
    }
}

int CheckButtonsStatus( button_t* buttons, size_t size )
{
    for (size_t i = 0; i < size; i++)
    {
        if (buttons[i].is_invis == false)
        {

            if (CheckCollisionPointRec(GetMousePosition(), buttons[i].button_rect))
            {
                buttons[i].is_mouse_on = true;
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    return (int)i;
            }
            else
                buttons[i].is_mouse_on = false;
        }
    }
    return -1;
}

void DrawButtons( button_t* buttons, size_t size )
{
    for (size_t i = 0; i < size; i++)
    {
        if (buttons[i].is_invis == false)
        {
            DrawRectangleRec(buttons[i].button_rect, (buttons[i].is_mouse_on == true ?  buttons[i].mouse_on_color : buttons[i].common_color));
            DrawText(buttons[i].button_text, buttons[i].TextX, buttons[i].TextY, buttons[i].font, MAROON);
        }
    }
}


position_t ButtonTextPosition( Rectangle button, const char* text, int font )
{
    int text_width = MeasureText(text, font);
    return {(int)button.x + ((int)button.width - text_width) / 2, (int)button.y + ((int)button.height - font) / 2};
}

void SetButtonsTextPosition(button_t* buttons, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        position_t text_position = ButtonTextPosition(buttons[i].button_rect, buttons[i].button_text, buttons[i].font);
        buttons[i].TextX = text_position.x;
        buttons[i].TextY = text_position.y;
    }
}



void DrawInputBar( int top_y, int text_font, Color color )
{
    Rectangle text_box = {0.1 * SCREEN_WIDTH, (float)top_y, 0.68 * SCREEN_WIDTH, (float)(text_font + 10)};
    bool is_mouse_on_text = false;

    if (CheckCollisionPointRec(GetMousePosition(), text_box))
        is_mouse_on_text = true;
    else
        is_mouse_on_text = false;

    if (is_mouse_on_text)
    {
        int key = GetCharPressed();

        while (key > 0)
        {
            if ((key >= 32) && (key <= 125) && (letters < 99))
            {
                string[letters] = (char)key;
                letters++;
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE))
        {
            letters--;
            if (letters < 0)
                letters = 0;
            string[letters] = '\0';
        }
    }


    DrawRectangleRec(text_box, color);
    for (int i = 0; i < letters; i++)
    {
        if ((float)MeasureText(string + i, text_font) < text_box.width)
        {
            DrawText(string + i, (int)text_box.x + 5, (int)text_box.y + 5, text_font, MAROON);
            break;
        }
    }
}

