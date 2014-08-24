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
    //const float initfloats[] = {320.f, 240.f, 0.f, 0.f};
    vm.init(4, 1000);

    std::ifstream file("code.txt");
    std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    std::string error;
    if(!wvm::assemble(code.c_str(), vm.program, &error))
    {
        std::printf("%s\n", error.c_str());
        return 1;
    }
    vm.findSubprograms();

    vm.globals.push_back(320.f); //-1 is x
    vm.globals.push_back(240.f); //-2 is y
    vm.globals.push_back(M_PI); //-3 is pi
    vm.globals.push_back(-M_PI); //-4 is -pi
    vm.globals.push_back(0.5f); //-5 is 0.5f
    vm.globals.push_back(2.f); //-6 is 2.f

    vm.runVmProgram(0); //init all particles

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
        }

        vm.runVmProgram(1);

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

