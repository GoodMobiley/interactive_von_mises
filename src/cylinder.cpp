#include <MISES/cylinder.hpp>

#include <MISES/von_mises.hpp>
#include <MISES/options.hpp>
#include <MISES/airfoil.hpp>

namespace mises{

class MovableZero : public Complex, public Movable {
  public:
    MovableZero(const Complex value) : Complex(value) {}

    Point get_relative_position(const Point point) const {
        const Point position = Cylinder::NormalizedPointToFramePoint(
            Cylinder::ComplexToNormalizedPoint(*this)
        );

        return position - point;
    }

    void set_position(const Point position){
        Cylinder::SetZero(this, Cylinder::NormalizedPointToComplex(
            Cylinder::FramePointToNormalizedPoint(position)
        ));
    }
};

class MovableXiAxis : public Movable {
  public:
    Point get_relative_position(const Point point) const {
        const Point position = Cylinder::NormalizedPointToFramePoint(
            Cylinder::ComplexToNormalizedPoint(Complex(0.0, 0.0))
        );        

        return {
            0.0,
            position.y - point.y
        };
    }

    void set_position(const Point position){
        Point norm_position = Cylinder::FramePointToNormalizedPoint(position);
        Point origin = Cylinder::ComplexToNormalizedPoint(Complex(0.0, 0.0));

        Cylinder::SetOrigin({
            origin.x,
            norm_position.y
        });
    }

    GLFWcursor* hover_cursor(){
        static GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);

        return cursor;
    }

    GLFWcursor* drag_cursor(){
        return hover_cursor();
    }
};

class MovableEtaAxis : public Movable {
  public:
    Point get_relative_position(const Point point) const {
        const Point position = Cylinder::NormalizedPointToFramePoint(
            Cylinder::ComplexToNormalizedPoint(Complex(0.0, 0.0))
        );        

        return {
            position.x - point.x,
            0.0
        };
    }

    void set_position(const Point position){
        Point norm_position = Cylinder::FramePointToNormalizedPoint(position);
        Point origin = Cylinder::ComplexToNormalizedPoint(Complex(0.0, 0.0));

        Cylinder::SetOrigin({
            norm_position.x,
            origin.y
        });
    }

    GLFWcursor* hover_cursor(){
        static GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);

        return cursor;
    }

    GLFWcursor* drag_cursor(){
        return hover_cursor();
    }
};

class MovableOrigin : public Movable {
  public:
    Point get_relative_position(const Point point) const {
        const Point position = Cylinder::NormalizedPointToFramePoint(
            Cylinder::ComplexToNormalizedPoint(Complex(0.0, 0.0))
        );

        return position - point;
    }

    void set_position(const Point position){
        Cylinder::SetOrigin(
            Cylinder::FramePointToNormalizedPoint(position)
        );
    }

    GLFWcursor* hover_cursor(){
        static GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);

        return cursor;
    }

    GLFWcursor* drag_cursor(){
        return hover_cursor();
    }
};

}

using namespace mises;

MovableXiAxis XiAxis;
MovableEtaAxis EtaAxis;
MovableOrigin Origin;

Rect Cylinder::Frame;

Movable* Cylinder::SelectedItem = nullptr;
Point Cylinder::CursorPos;
Point Cylinder::RelativeSelectionPos;
bool Cylinder::Mouse1Hold = false;

double  Cylinder::A = 1.0;
Complex Cylinder::Mu(0.0, 0.0);
std::vector<Complex*> Cylinder::Zeros = {
    new Complex(1.0, 0.0),
    new MovableZero(Complex(-1.0, 0.0))
};

void Cylinder::CursorPosCallback(GLFWwindow* window, double x_pos, double y_pos){
    CursorPos.x = x_pos;
    CursorPos.y = Frame.height - y_pos;

    if(!Mouse1Hold){
        UpdateSelection();
    }
    else if(SelectedItem){
        SelectedItem->set_position(CursorPos + RelativeSelectionPos);
    }

    UpdateCursor();
}

void Cylinder::MouseButtonCallback(GLFWwindow* window, int button, int action, int mod){
    if(button == GLFW_MOUSE_BUTTON_1){

        if(action == GLFW_PRESS){
            if(!SelectedItem){
                const double min_dim = Frame.width > Frame.height ? Frame.height : Frame.width;
                const double coeff = 1.0 / (0.5 * min_dim - MISES_PADDING);

                const double x_norm = (CursorPos.x - 0.5 * Frame.width)  * coeff;
                const double y_norm = (CursorPos.y - 0.5 * Frame.height) * coeff;

                AddZero(Complex(
                    x_norm * A + Mu.real(),
                    y_norm * A + Mu.imag()
                ));
            }

            Mouse1Hold = true;
        }
        else if(action == GLFW_RELEASE){
            Mouse1Hold = false;
        }
    }
    else if(button == GLFW_MOUSE_BUTTON_2){
        if(SelectedItem){
            RemoveZero(static_cast<MovableZero*>(SelectedItem));            
        }
    }

    UpdateSelection();
    UpdateCursor();
}

Rect Cylinder::GetFrameRect(){
    return Frame;
}

void Cylinder::SetFrameRect(const Rect frame){
    Frame = frame;
}

Point Cylinder::ComplexToNormalizedPoint(const Complex value){
    return {
        (value.real() - Mu.real()) / A,
        (value.imag() - Mu.imag()) / A
    };
}

Complex Cylinder::NormalizedPointToComplex(const Point norm_point){
    return Complex(
        norm_point.x * A + Mu.real(),
        norm_point.y * A + Mu.imag()
    );
}

Point Cylinder::NormalizedPointToFramePoint(const Point norm_point){
    const double min_dim = Frame.width > Frame.height ? Frame.height : Frame.width;
    const double coeff = 0.5 * min_dim - MISES_PADDING;

    return {
        0.5 * Frame.width  + norm_point.x * coeff,
        0.5 * Frame.height + norm_point.y * coeff
    };
}

Point Cylinder::FramePointToNormalizedPoint(const Point frame_point){
    const double min_dim = Frame.width > Frame.height ? Frame.height : Frame.width;
    const double coeff = 1.0 / (0.5 * min_dim - MISES_PADDING);

    return {
        (frame_point.x - 0.5 * Frame.width)  * coeff,
        (frame_point.y - 0.5 * Frame.height) * coeff
    };
}

double Cylinder::GetA(){
    return A;
}

Complex Cylinder::GetMu(){
    return Mu;
}

void Cylinder::SetMu(const Complex mu){
    Mu = mu;
    A = std::abs(Complex(1.0, 0.0) - Mu);

    OnMuUpdate();
}

void Cylinder::SetOrigin(const Point norm_point){
    double mu_real;
    double mu_imag;
    
    if(norm_point.x == 0.0){
        mu_real = 0.0;
        mu_imag = (norm_point.y >= 0? -1.0 : 1.0) * std::sqrt(norm_point.y*norm_point.y / (1.0 - norm_point.y*norm_point.y));
    }
    else{
        const double mu_c = (norm_point.y*norm_point.y - 1.0) / (norm_point.x*norm_point.x) + 1.0;
        mu_real = (1.0 + (norm_point.x >= 0 ? 1.0: -1.0) * std::sqrt(1.0 - mu_c)) / mu_c;
        mu_imag = mu_real * norm_point.y / norm_point.x; 
    }

    SetMu(Complex(mu_real, mu_imag));
}

std::vector<Complex> Cylinder::GetZeros(){
    std::vector<Complex> zeros;
    zeros.reserve(Zeros.size());

    for(size_t i = 0; i < Zeros.size(); ++i){
        zeros.push_back(*Zeros[i]);
    }

    return zeros;
}

bool Cylinder::AddZero(const Complex value){
    if(Zeros.size() >= MISES_MAX_ZERO_COUNT){
        return false;
    }

    const size_t count = Zeros.size() - 1;
    const Complex zero_offset = -value / Complex(count, 0.0);

    for(size_t i = 0; i < count; ++i){
        *Zeros[i+1] += zero_offset;
    }

    Zeros.push_back(new MovableZero(value));

    CorrectZeros();

    return true;
}

bool Cylinder::SetZero(Complex* const zero, const Complex value){
    if(Zeros.size() <= 2) return false;

    bool has_zero = false;
    for(size_t i = 1; i < Zeros.size(); ++i){
        if(Zeros[i] == zero){
            has_zero = true;
            break;
        }
    }
    if(!has_zero) return false;

    const size_t count = Zeros.size() - 1;
    const Complex zero_offset = (*zero - value) / Complex(count, 0.0);

    for(size_t i = 0; i < count; ++i){
        if(Zeros[i+1] == zero){
            *Zeros[i+1] = value;
        }
        else{
            *Zeros[i+1] += zero_offset;
        }
    }

    CorrectZeros();

    return true;
}

bool Cylinder::RemoveZero(Complex* const zero){
    if(Zeros.size() <= 2) return false;

    size_t zero_index = 0;
    for(size_t i = 1; i < Zeros.size(); ++i){
        if(Zeros[i] == zero){
            zero_index = i;
            break;
        }
    }
    if(!zero_index) return false;

    Zeros.erase(Zeros.begin() + zero_index);

    const size_t count = Zeros.size() - 1;
    const Complex zero_offset = *zero / Complex(count, 0.0);

    for(size_t i = 0; i < count; ++i){
        *Zeros[i+1] += zero_offset;
    }

    CorrectZeros();

    delete static_cast<MovableZero*>(zero);

    return true;
}

void Cylinder::CorrectZeros(){
    Complex zero_center = Complex(0.0, 0.0);

    for(Complex* zero: Zeros){
        zero_center += *zero;
    }

    if(zero_center == Complex(0.0, 0.0)) {
        OnZeroUpdate();
        return;
    }

    const size_t count = Zeros.size() - 1;
    const Complex zero_offset = -zero_center / Complex(count, 0.0);

    for(size_t i = 0; i < count; ++i){
        *Zeros[i+1] += zero_offset;
    }

    OnZeroUpdate();
}

void Cylinder::UpdateSelection(){
    Movable* closest_item = &Origin;
    double distance;
    double closest_distance = closest_item->get_relative_position(CursorPos).norm();

    for(size_t i = 1; i < Zeros.size(); ++i){
        MovableZero* zero = static_cast<MovableZero*>(Zeros[i]);
        distance = zero->get_relative_position(CursorPos).norm(); 
        if(distance < closest_distance){
            closest_item = zero;
            closest_distance = distance;
        }
    }

    if(closest_distance <= MISES_SELECTION_RADIUS){
        SelectedItem = closest_item;
        RelativeSelectionPos = SelectedItem->get_relative_position(CursorPos);
        return;
    }

    distance = XiAxis.get_relative_position(CursorPos).norm();
    if(distance < closest_distance){
        closest_item = &XiAxis;
        closest_distance = distance;
    }

    distance = EtaAxis.get_relative_position(CursorPos).norm();
    if(distance < closest_distance){
        closest_item = &EtaAxis;
        closest_distance = distance;
    }

    if(closest_distance <= MISES_SELECTION_RADIUS){
        SelectedItem = closest_item;
        RelativeSelectionPos = SelectedItem->get_relative_position(CursorPos);
    }
    else{
        SelectedItem = nullptr;
    }
}

void Cylinder::UpdateCursor(){
    GLFWcursor* cursor = NULL;

    if(SelectedItem){
        if(Mouse1Hold){
            cursor = SelectedItem->drag_cursor();
        }
        else{
            cursor = SelectedItem->hover_cursor();
        }
    }

    glfwSetCursor(glfwGetCurrentContext(), cursor);
}

void Cylinder::OnZeroUpdate(){
    Airfoil::CalculateCoefficients();
    mises::RenderFrame();
}

void Cylinder::OnMuUpdate(){
    Airfoil::CalculateVertices();
    mises::RenderFrame();
}