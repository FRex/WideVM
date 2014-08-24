/* 
 * File:   main.cpp
 * Author: frex
 *
 * Created on August 24, 2014, 12:43 AM
 */

#include <random>
#include <SFML/Graphics.hpp>

#include "WideVM.hpp"

int main(int argc, char** argv)
{
    std::mt19937 twister;
    std::uniform_real_distribution<float> distr(-M_PI, M_PI);
    std::uniform_real_distribution<float> speeddistr(0.5f, 2.f);

    sf::RenderWindow app(sf::VideoMode(640u, 480u), "wide vm");
    app.setFramerateLimit(60u);

    wvm::WideVM vm;
    const float initfloats[] = {320.f, 240.f, 0.f, 0.f};
    vm.init(4, 1000, initfloats);

    for(int i = 0; i < vm.particleCount(); ++i)
    {
        float * ptr = vm.getParticle(i);
        const float angle = distr(twister);
        const float speed = speeddistr(twister);
        ptr[2] = speed * std::cos(angle);
        ptr[3] = speed * std::sin(angle);
    }

    vm.loadProgram("program.txt");

    sf::VertexArray arr(sf::Points, vm.particleCount());

    while(app.isOpen())
    {
        sf::Event eve;
        while(app.pollEvent(eve))if(eve.type == sf::Event::Closed) app.close();

        vm.runVmProgram();

        app.clear();
        for(int i = 0; i < vm.particleCount(); ++i)
        {
            const float * ptr = vm.getParticle(i);
            arr[i].position.x = ptr[0];
            arr[i].position.y = ptr[1];
        }
        app.draw(arr);
        app.display();
    }//while app isOpen
}

