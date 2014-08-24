/* 
 * File:   main.cpp
 * Author: frex
 *
 * Created on August 24, 2014, 12:43 AM
 */

#include <cstdlib>
#include <vector>
#include <cstdio>
#include <fstream>
#include <string>
#include <cstring>
#include <random>
#include <SFML/Graphics.hpp>

enum EVM_OPCODE
{
    EVO_ACCUMULATE = 1,
};

class WideVM
{
public:
    std::vector<float> data;
    std::vector<char> program;

    int particlesize = 2;
    int pc;

    void init(int size, int count, const float * initvals = nullptr)
    {
        particlesize = size;
        data.resize(size*count, 0.f);
        if(initvals)
        {
            for(int i = 0; i < particleCount(); ++i)
            {
                std::memcpy(getParticle(i), initvals, sizeof (float)* size);
            }
        }
    }

    void runVmProgram()
    {
        pc = 0;
        while(pc < program.size())
        {
            switch(program[pc])
            {
                case EVO_ACCUMULATE:
                    opAccumulate();
                    break;
            } //switch opcodes pc
            ++pc;
        } //while pc < opcodes size
    }

    int particleCount() const
    {
        return data.size() / particlesize;
    }

    float * getParticle(int index)
    {
        return data.data() + index * particlesize;
    }

    void opAccumulate()
    {
        const int accu = program[++pc];
        const int operand = program[++pc];

        for(int i = 0; i < particleCount(); ++i)
        {
            float * ptr = getParticle(i);
            ptr[accu] += ptr[operand];
        }
    }

    void loadProgram(const std::string& filename)
    {
        std::ifstream file(filename.c_str());
        int opcode;
        program.clear();
        while(file >> opcode)
            program.push_back(opcode);
    }

private:


};

int main(int argc, char** argv)
{
    std::mt19937 twister;
    std::uniform_real_distribution<float> distr(-M_PI, M_PI);
    std::uniform_real_distribution<float> speeddistr(0.5f, 2.f);

    sf::RenderWindow app(sf::VideoMode(640u, 480u), "wide vm");
    app.setFramerateLimit(60u);

    WideVM vm;
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

