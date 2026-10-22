#pragma once

#include <ftxui/dom/elements.hpp>

namespace mentor::tui{

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

inline ftxui::Element getWelcomePage()
{
    using namespace ftxui;
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
}

}