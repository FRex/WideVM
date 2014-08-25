/* 
 * File:   main.cpp
 * Author: frex
 *
 * Created on August 24, 2014, 12:43 AM
 */

#include <fstream>
#include <iterator>
#include <SFML/Graphics.hpp>

#include "WideVM.hpp"
#include "Assembler.hpp"

int main(int argc, char** argv)
{
    int fpssamplecounter = 0;
    float fpssamples[100];
    float fps = 0.f;

    sf::RenderWindow app(sf::VideoMode(640u, 480u), "wide vm");
    app.setFramerateLimit(60u);

    wvm::WideVM vm;

    std::ifstream file("program.txt");
    std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    std::string error;
    if(!vm.loadAsmProgram(code, &error))
    {
        std::printf("%s\n", error.c_str());
        return 1;
    }

    vm.addParticles(1000, "init"); //init with 1000 particles

    sf::VertexArray arr(sf::Points, vm.particleCount());

    const wvm::VMLocation x = vm.getFieldLocation("x");
    const wvm::VMLocation y = vm.getFieldLocation("y");

    while(app.isOpen())
    {
        sf::Clock clock;

        //handle events:
        sf::Event eve;
        while(app.pollEvent(eve))
        {
            if(eve.type == sf::Event::Closed) app.close();
            if(eve.type == sf::Event::KeyPressed)
            {
                if(eve.key.code == sf::Keyboard::Escape) app.close();
            }
            if(eve.type == sf::Event::MouseButtonPressed)
            {
                //set up the globals for next prog:
                vm.setGlobal("x", eve.mouseButton.x);
                vm.setGlobal("y", eve.mouseButton.y);
                //run  prog on 100 new particles
                vm.addParticles(100, "init");
                arr.resize(arr.getVertexCount() + 100u);
            }
        }

        //run vm and draw particles:
        vm.runVmProgram("run");
        app.clear();
        for(int i = 0; i < vm.particleCount(); ++i)
        {
            wvm::Particle par = vm.getParticle(i);
            arr[i].position.x = par.getField(x);
            arr[i].position.y = par.getField(y);
        }
        app.draw(arr);
        app.display();

        //calc fps:
        fpssamples[fpssamplecounter] = clock.getElapsedTime().asSeconds();
        ++fpssamplecounter;
        if(fpssamplecounter == 100)
        {
            fps = 0.f;
            for(int i = 0; i < 100; ++i)
                fps += fpssamples[i];

            fps = 100.f / fps;
            fpssamplecounter = 0;
            std::printf("FPS: %f\nPARTICLES: %d\n", fps, vm.particleCount());
        }
    }//while app isOpen
}

