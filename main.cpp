#include <iostream>
#include <core/Engine.h>

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main(int argc, char* argv[])
{
    AlkyoneRenderEngine* engine = new AlkyoneRenderEngine();

    if (engine->Initialize() == false)
    {
        exit(EXIT_FAILURE);
    } else
    {
        engine->Run();
        engine->Terminate();
    }

    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}
