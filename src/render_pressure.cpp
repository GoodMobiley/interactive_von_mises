#include <MISES/pressure.hpp>

#include <MISES/pallet.hpp>

using namespace mises;

void Pressure::Initialize(){

}

void Pressure::RenderFrame(){
    Pallet::Create(Frame);

    DrawGrid();
    DrawAxes();
    DrawPressure();
}

void Pressure::DrawGrid(){

}

void Pressure::DrawAxes(){

}

void Pressure::DrawPressure(){
    
}

