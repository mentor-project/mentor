#pragma once

#include "Basic/AppContext.hpp"
#include <ftxui/component/app.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace mentor::tui
{

static inline std::string_view logo = R"(                                                                   
                                                                   
          ____                          ___                        
        ,'  , `.                      ,--.'|_                      
     ,-+-,.' _ |              ,---,   |  | :,'    ,---.    __  ,-. 
  ,-+-. ;   , ||          ,-+-. /  |  :  : ' :   '   ,'\ ,' ,'/ /| 
 ,--.'|'   |  || ,---.   ,--.'|'   |.;__,'  /   /   /   |'  | |' | 
|   |  ,', |  |,/     \ |   |  ,"' ||  |   |   .   ; ,. :|  |   ,' 
|   | /  | |--'/    /  ||   | /  | |:__,'| :   '   | |: :'  :  /   
|   : |  | ,  .    ' / ||   | |  | |  '  : |__ '   | .; :|  | '    
|   : |  |/   '   ;   /||   | |  |/   |  | '.'||   :    |;  : |    
|   | |`-'    '   |  / ||   | |--'    ;  :    ; \   \  / |  , ;    
|   ;/        |   :    ||   |/        |  ,   /   `----'   ---'     
'---'          \   \  / '---'          ---`-'                      
                `----'                                             
                                                                   )";

inline ftxui::Component getWelcomePage(AppContext &ac)
{
    using namespace ftxui;
    return Renderer([]
                    {
                        return vbox({
                                   filler(),
                                   hbox({
                                       filler(),
                                       text(logo) | bold | color(Color::RGB(0, 240, 255)),
                                       filler(),
                                   }),
                                   filler(),
                               }) |
                               bgcolor(Color::Black);
                    });
}

inline bool handleWelcomePageEvent(AppContext &ac, const ftxui::Event &e)
{
    if (e == ftxui::Event::Return)
    {
        ac.pm.navigate(Page::select_cur_dir);
        return true;
    }
    return false;
}

} // namespace mentor::tui
