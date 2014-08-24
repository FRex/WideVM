/* 
 * File:   main.cpp
 * Author: frex
 *
 * Created on August 24, 2014, 12:43 AM
 */

#include <random>
#include <cmath>
#include <fstream>
#include <iterator>
#include <SFML/Graphics.hpp>

#include "WideVM.hpp"
#include "Assembler.hpp"

int main(int argc, char** argv)
{
    std::srand(std::time(nullptr));

    std::mt19937 twister;
    std::uniform_real_distribution<float> distr(-M_PI, M_PI);
    std::uniform_real_distribution<float> speeddistr(0.5f, 2.f);

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

    while(app.isOpen())
    {
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
                vm.globals[0] = eve.mouseButton.x;
                vm.globals[1] = eve.mouseButton.y;
                //run 0 prog on 100 new particles
                vm.addParticles(100, "init");
                arr.resize(arr.getVertexCount() + 100u);
            }
        }

        vm.runVmProgram("run");

        app.clear();
        for(int i = 0; i < vm.particleCount(); ++i)
        {
            const float * ptr = vm.getParticle(i);
            if(i == 0) std::printf("%f %f %f %f\n", ptr[0], ptr[1], ptr[2], ptr[3]);


            arr[i].position.x = ptr[0];
            arr[i].position.y = ptr[1];
        }
        app.draw(arr);
        app.display();
    }//while app isOpen
}

