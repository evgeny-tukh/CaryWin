#pragma once

#include <exception>
#include <Windows.h>

namespace Cary {
    class App {
        public:
            App (HINSTANCE _inst): inst (_inst) {
                if (app) {
                    throw std::exception ();
                }

                app = this;
            }

            void run ();
            static App *getApp () { return app; }
            static HINSTANCE getInst () { return app->inst; }
            
        private:
            static App *app;
            HINSTANCE inst;
    };
}