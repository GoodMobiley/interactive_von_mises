#include <MISES/velocities.hpp>

#include <MISES/pallet.hpp>

using namespace mises;

void Velocities::Initialize(){

}

void Velocities::RenderFrame(){
    Pallet::Create(Frame);

    DrawGrid();
    DrawAxes();
    DrawPressure();
}

void Velocities::DrawGrid(){

}

void Velocities::DrawAxes(){

}

void Velocities::DrawPressure(){
    
}

