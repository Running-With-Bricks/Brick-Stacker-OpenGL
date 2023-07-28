
# Brick-Stacker
## Project is discontinued
Feel free to look at the code, fork it, modify and use it for whatever reasons you want. Crediting me would be appreciated. Also at the bottom there is Brick-Stacker's History if you care.

## About

Blazingly Fast Light-Weight workshop for [Brick-Hill](https://www.brick-hill.com) written in C++, using OpenGL.

  

## How to build

Brick-Stacker uses [Premake 5.0](https://premake.github.io) to allow easy project generation across all platforms.

For more info on building with Premake use this [link](https://premake.github.io/docs/Using-Premake)

### Windows

You can go to `./scripts` and run `build_windows.bat`, it will generate solution files for Visual Studio 2022.

You can edit the bat file to generate code for different Visual Studio version following this [tutorial](https://premake.github.io/docs/Using-Premake).

### MacOS

Fuck Mac.

### Linux

BUILDING LINUX WILL FAIL(please help me)

Right now, there is no automatic solution, so you will have to build it youself.

[Download](https://premake.github.io/download) Premake and then, using downloaded binary, build it.

You are using Linux, so should be able to figure it out yourself.

## Histroy ig
so
On September 11 2023, in MixaHosting Dev server, 1ris was building MixaHosting map, which was never released and I think also never was finished, but 1ris has posted this image.
![the start](https://cdn.discordapp.com/attachments/1022461044985167972/1134038255843680356/image.png)
And I made this remark about it.
![this is due to one of the globes that appear in the game FEZ](https://cdn.discordapp.com/attachments/1022461044985167972/1134038868841213992/image.png)
While I tried to build it, I gave up after placing 4 bricks, because BrickBuilder was not enjoyable for me to use.
During September 17-18 2022, the weekends, I decided to create a brk loader in Godot. I never used Godot before, so this was to try it out. I hated and still hate Godot. 
And on next day I had this conversation
![a](https://cdn.discordapp.com/attachments/1022461044985167972/1134041255131746395/image.png)![b](https://cdn.discordapp.com/attachments/1022461044985167972/1134041423986036816/image.png)
After that I tried making the workshop using Monogame C# framework.
During this stage of development, the name was chosen.
![the name](https://cdn.discordapp.com/attachments/1022461044985167972/1134045006383026196/image.png)
The name was later changed to "Brick-Stacker" tho.
It ended with adding ImGui, about after a month, due to ImGui bindings being really wonky and breaking 3d rendering.
This image is good summary of the Monogame Brick-Stacker's end
![end of monogame](https://cdn.discordapp.com/attachments/1022461044985167972/1134043828261748736/image.png)
And so the monogame was dropped.
On October 1st 2022, I, guess what, tried to using C++ and OpenGL again, got the triangle drawing, but that idea was imediatly dropped for a one day idea to make, I kid you not, Vulkan Raytracer. This idea was dropped the very next day. And now, normal Vulkan renderer Brick-Stacker. Remember that this guy had at the time **no c++ and graphics programming experience**
For first week nothing happened, but then I got a triangle to draw and then I tried embedding ImGui...
This gif was accompanied with comment "it works!"
![it works!](https://gyazo.com/1e2c684f3d995468b729fbd5098cd090.gif)I never ended up fully fixing the imgui implementation, however the flickering was removed.
And almost now work hase been done until end of that October
Here's more indev foutage from dating 28th oct and couple of days later
[Editable Parameters](https://gyazo.com/0a2ba90cd3159e5061a339874036d05f.mp4)
[This Gif spawned some funny justs about 4d, but it's showcase of 3d](https://cdn.discordapp.com/attachments/1022461044985167972/1036001335440572447/2022-10-29_22-37-23_2.mp4)
![2030](https://cdn.discordapp.com/attachments/1022461044985167972/1134050436303814717/image.png)
[a little bit more fun - Voron](https://cdn.discordapp.com/attachments/1022461044985167972/1036004130973569114/2022-10-29_22-49-55.mp4)
![FEZ](https://cdn.discordapp.com/attachments/1022461044985167972/1134051150430228531/image.png)
If you want to toy around with this 3d test for some reason, [go ahead](https://cdn.discordapp.com/attachments/1022461044985167972/1036013689888841808/stacker.rar). This is the first ever exe I shared with the people you see in screenshots. Note that you might have issues with running this one due to probably compilation issues. MixaMega wasn't able to run it at all, while Yek was able to run it, but resizing the windows was broken. This issue is persistant in all Vulkan versions and I still don't know the root cause.
Video of camera working.
[camera done. I fogot to show case the fov tho - Voron](https://cdn.discordapp.com/attachments/1022461044985167972/1036213701402959933/2022-10-30_11-40-47_2.mp4)
![piss n shit](https://cdn.discordapp.com/attachments/1022461044985167972/1134121052323196949/image.png)
[holy moly - MixaMega; yo im shitting my pants - Yek](https://cdn.discordapp.com/attachments/1022461044985167972/1036319342670905475/2022-10-30_18-37-23_2.mp4)
As seen in the video, the rotations of bricks are fucked up. Well big thanks to Ty/TyThePotato/TyJupiter (idk which is correct) for giving me explanation on how to properly implement scaling and rotation (and I gotta say, whoever came up with this was not thinking writing the code).
Next progress update, fixed rotations were done on Nov 7th. [video](https://cdn.discordapp.com/attachments/1022461044985167972/1039209853547204608/2022-11-07_18-05-09_2.mp4)
Also I made this plan, it was updated at some point.
![plan](https://cdn.discordapp.com/attachments/1022461044985167972/1134182690942361710/image.png)
But OpenGL makes a return for **third** time!
![the idea](https://cdn.discordapp.com/attachments/1022461044985167972/1134185136888492232/image.png)![and a stupid idea by yek](https://cdn.discordapp.com/attachments/1022461044985167972/1134185599092391946/image.png)
And then I didn't
10th of Nov - update of image when resizing
![Resizing](https://gyazo.com/d589ea9265a2e3b595db9ffa38fa7793.gif)
11th of November - I decided to discontinue Brick-Stacker-Vulkan and post this video to Brick Hill's workshop creations in the discord server: [link to original post](https://discord.com/channels/267810707574226964/300831680216694784/1040666647553392700), [link to brick hill discord](https://discord.gg/brick-hill) (make sure you give yourself role to view archived channels)
[Video standalone](https://cdn.discordapp.com/attachments/300831680216694784/1040666646848745612/BrickStacker.mp4)

> Here's small showcase of small project which is kinda a workshop, but
> with many features missing(like transparency) and unoptimised. Made
> with no game engine in C++ and Vulkan Map brick count - 1130 Average
> fps - 60(it is not fps locked, just unoptimised) GPU usage - 15%(AMD
> Radeon(TM) Vega 8 Graphics)

Calling this "small project" was a big understatement Considering it took me 1 month, including procrustination, but not monogame or godot.

and so I have layed my eyes on OpenGL for 4th time, now, for the last time.
Nov 12th
![the start of an end](https://cdn.discordapp.com/attachments/1022461044985167972/1134187968903532644/image.png)
Now I won't be more into details as you can look at, ah, very badly named commit history, but at least you can see the changes.

But here's more dev screenshots, gifs, vids
nov26
![3d](https://cdn.discordapp.com/attachments/1022461044985167972/1046102254802972834/image.png)
Imgui dec3
![imgui again](https://cdn.discordapp.com/attachments/1022461044985167972/1048545827171356722/image.png)
Roots of modern camera dec10, but I claimed that I've done it long ago.
[video](https://cdn.discordapp.com/attachments/1022461044985167972/1051165977590640710/2022-12-10_17-55-39_2.mp4)
Some really interesting looking "skybox" dec10
![skybox](https://media.discordapp.net/attachments/1022461044985167972/1051241404791275580/IMG20221210225322.jpg?width=1202&height=676) 
also dec22 - jan8 was winter building contest, so little to no progress was done, so here's amazing concept art for one of the ideas in our winning game "Snowmen Showdown"
dec30
![bossfight](https://cdn.discordapp.com/attachments/914231509181931614/1134193530286383104/image.png) 
denoiser is creepy - MixaMega; jan7
![denoiser](https://cdn.discordapp.com/attachments/1022461044985167972/1061345183662358558/blender_CIjQGWyopT.png)
Work on Stacker continued on jan 19th.
On jan25 I posted this list of what has to be done. To be clear, I never returned to it.
![bad plan](https://media.discordapp.net/attachments/1022461044985167972/1134425214546092092/image.png)
![explanation](https://cdn.discordapp.com/attachments/1022461044985167972/1134425214944555008/image.png)
feb6 - Textures added!
![textures](https://cdn.discordapp.com/attachments/1022461044985167972/1134488154032832603/image.png)
Also this is second rar that I shared with, except here I shared it because there was a bug with shaders, the fix is explained below, you can change the .frag shader to remove it.
[Brick-Stacker-Textured](https://cdn.discordapp.com/attachments/1022461044985167972/1072549798273630268/Debug-windows-x86_64.rar)
![explanation](https://cdn.discordapp.com/attachments/1022461044985167972/1134490106183553114/image.png)
On feb12, the name was changed.
![better name](https://cdn.discordapp.com/attachments/1022461044985167972/1134492014751268964/image.png)
Also this date was the first time Brick-Stacker was mentioned publically.
![publicity](https://cdn.discordapp.com/attachments/1022461044985167972/1074397589618823282/image.png)
March 20th was date when I added camera also I think some framerate improvements(prob just building in Release mode lol).
[rar](https://cdn.discordapp.com/attachments/1022461044985167972/1087430158652276746/Release-windows-x86_64.rar)
May 3rd - I added a logo to the app, prob copyrighted
![logo](https://cdn.discordapp.com/attachments/1022461044985167972/1134500151151571045/image.png)
May 22 - Open/Saving brks, also some time before I started using ECS, but you don't have to care.
[rar](https://cdn.discordapp.com/attachments/1022461044985167972/1110296115842121852/Release-windows-x86_64.rar)
31st May - Camera Switching Showcase
[video](https://cdn.discordapp.com/attachments/1022461044985167972/1113557154075512922/Brick-Stacker-OpenGL_r1grbwDtpG.mp4)
June 2nd - Yek Review, very integral to the development, really really
![the review](https://cdn.discordapp.com/attachments/1022461044985167972/1134502719135166615/image.png)
June 6 - Transparency ordering
[rar](https://cdn.discordapp.com/attachments/1022461044985167972/1115721808210043032/Release-windows-x86_64.rar)
June 21 - Selecting bricks
[rar](https://cdn.discordapp.com/attachments/1022461044985167972/1121149678378045543/Release-windows-x86_64.rar)

And this is where the development process gets a stroke, I intended to work on multiselection first, but it was really hard, had no clue how to approach it and etc. But then I got nominated for Best Scripter in Sir Stephen's Brick Hill Awards 2023, and I wanted to get something for my campaign, so I've chosen to promote Brick-Stacker, but with a twist - noone knew what Brick-Stacker is. Yes, I, for fun, decided to run a campaign where noone knew what I was speaking about. 

> "i see. i thought brick stacker was a tetris clone or something eecks dee"
>  Tylorfoot

So I started creating "Minimal Viable Product version of Brick-Stacker", this meant creating a usable product, but clearly with room for improvement. And since selecting multiple bricks isn't shiny enough, I locked away multiselection in favour for gizmos. Well I didn't have enough time to actually finish the mvp version and the awards were cancelled. But for awards I did create this video.
[video](https://cdn.discordapp.com/attachments/788877127176814635/1129819601920872468/0001-5912.mp4)

And this is it. The end. The latest commit was done to push latest code, not a working code.
Thanks to everyone involved, who supported, and people who's libraries I used.

## Credits
|People | Contribution |
|--|--|
|Voron | literally everything |
|Dargy | GitHub actions|
|[Ty](https://github.com/TyThePotato) | Helping fixing the rotations |


[premake5.0](https://premake.github.io/) - Build configuration
[glad](https://glad.dav1d.de/) - OpenGL bindings
[glfw](https://www.glfw.org/) - Windowing
[glm](https://github.com/g-truc/glm) - Math
[ImGui](https://github.com/ocornut/imgui) - GUI
[spdlog](https://github.com/gabime/spdlog) - Logging library
[stb_image](https://github.com/nothings/stb) - Image loading
[EnTT](https://github.com/skypjack/entt) - Entity Component System
[Jolt Physics](https://github.com/jrouwe/JoltPhysics) - Physics, used for brick selection

Yek is a cunt
