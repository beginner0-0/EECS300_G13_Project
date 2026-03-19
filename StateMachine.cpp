#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <optional>


int counter = 0;

class StateMachine {
public:
    enum class State {
        Init,
        Waiting,
        Top,
        Bot,
        Plus,
        Minus
    };

    enum class Event {
        TopDetect,
        BotDetect,
        Clear,
        Enter,
        Leave
    };

    StateMachine() : currentState(State::Init) {}

    void handleEvent(Event event) {
        switch (currentState) {
            case State::Init:
                transitionTo(State::Waiting);
                break;

            case State::Waiting:
                if (event == Event::TopDetect) {
                    transitionTo(State::Top);
                } 
                else if (event == Event::BotDetect){
                    transitionTo(State::Bot);
                }

                else {
                    transitionTo(State::Waiting);
                }

                break;

            case State::Top:
                if (event == Event::Clear) {
                    transitionTo(State::Waiting);
                }
                else if (event == Event::Leave){
                    transitionTo(State::Minus);
                }
                else {
                    transitionTo(State::Top);
                }

                break;

            case State::Bot:
                if (event == Event::Clear) {
                    transitionTo(State::Waiting);
                }
                else if (event == Event::Enter){
                    transitionTo(State::Plus);
                }
                else {
                    transitionTo(State::Bot);
                }

                break;
            
            case State::Plus:
                transitionTo(State::Waiting);
                break;

            case State::Minus:
                transitionTo(State::Waiting);
                break;
        }
    }

    void update() {
        switch (currentState) {
            case State::Waiting:
                onWaiting();
                break;
            case State::Top:
                onTop();
                break;
            case State::Bot:
                onBot();
                break;
            case State::Init:
                onInit();
                break;
            case State::Minus:
                onMinus();
                break;
            case State::Plus:
                onPlus();
                break;
            
        }
    }

    State getState() const {
        return currentState;
    }

private:
    State currentState;

    void transitionTo(State newState) {
        onExit(currentState);
        currentState = newState;
        onEnter(currentState);
    }

    void onEnter(State state) {
        switch (state) {
            case State::Waiting:
                //std::cout << "Entering Waiting\n";
                break;
            case State::Top:
                //std::cout << "Entering Top\n";
                break;
            case State::Bot:
                //std::cout << "Entering Bot\n";
                break;
            case State::Init:
                //std::cout << "Entering Init\n";
                break;
            case State::Plus:
                //std::cout << "Entering Plus\n";
                break;
            case State::Minus:
                //std::cout << "Entering Minus\n";
                break;
        }
    }

    void onExit(State state) {
        switch (state) {
            case State::Waiting:
                //std::cout << "Exiting Waiting\n";
                break;
            case State::Top:
                //std::cout << "Exiting Top\n";
                break;
            case State::Bot:
                //std::cout << "Exiting Bot\n";
                break;
            case State::Init:
                //std::cout << "Exiting Init\n";
                break;
            case State::Plus:
                //std::cout << "Exiting Plus\n";
                break;
            case State::Minus:
                //std::cout << "Exiting Minus\n";
                break;
        }
    }

    void onWaiting() {
        std::cout << "waiting\n";
        std::cout << "counter = ";
        std::cout << counter;
        std::cout << "\n";
    }

    void onTop() {
        std::cout << "Top\n";
    }

    void onBot() {
        std::cout << "Bot\n";
    }

    void onInit() {
        std::cout << "Init\n";
    }

    void onPlus() {
        std::cout << "Plus\n";
        counter++;
    }
    
    void onMinus() {
        std::cout << "Minus\n";
        if (counter > 0) {
            counter--;
        }

    }
};

//We have this function because we need to pass an event into handleEvent(...) in int main, but want the event we pass in to be determined by a function
std::optional<StateMachine::Event> checkInputs(bool iinput1, bool iinput2, bool iinput3, bool iinput4, bool iinput5) {
    if (iinput1) {
        return StateMachine::Event::TopDetect;
    }
    if (iinput2) 
    {return StateMachine::Event::BotDetect;
    }
    if (iinput3) {
        return StateMachine::Event::Clear;
    }
    if (iinput4) 
    {return StateMachine::Event::Enter;
    }
    if (iinput5) {
        return StateMachine::Event::Leave;
    }
    return std::nullopt;   // no event
}


//variable I created to test different event
bool input1 = false;
bool input2 = false;
bool input3 = false;
bool input4 = false;
bool input5 = false;

class Keyboard {
private:
    termios oldt{};
    termios newt{};
    int oldf{};

public:
    Keyboard() {
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;

            // Disable canonical mode and echo
            newt.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);

            // Make stdin non-blocking
            oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
            fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    }
     ~Keyboard() {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);
    }
    bool keyAvailable(char& key) {
        int ch = getchar();
        if (ch != EOF) {
            key = static_cast<char>(ch);
            return true;
        }
        return false;
    }
};


void updateinputs(bool& q, bool& w, bool& e, bool& r, bool& t, Keyboard& kb) {
    char key;
    if (kb.keyAvailable(key)) {
        if (key == 'q') {
            q = !q;
            std::cout << "q = " << q << '\n';
        } 
        else if (key == 'w') {
            w = !w;
            std::cout << "w = " << w << '\n';
        } 
        else if (key == 'e') {
            e = !e;
            std::cout << "e = " << e << '\n';
        }
        else if (key == 'r') {
            r = !r;
            std::cout << "r = " << r << '\n';
        }
        else if (key == 't') {
            t = !t;
            std::cout << "t = " << r << '\n';
        }
    }
}

int main() {
    StateMachine sm;
    Keyboard kb;

    while (true) {
    
        updateinputs(input1, input2, input3, input4, input5, kb);
        usleep(1000);
        auto event = checkInputs(input1, input2, input3, input4, input5);

        //if (event.has_value()) {
        sm.handleEvent(*event);
        //}

        sm.update();
        std::cout << "TopDetect = "  << input1;
        std::cout << "\n";
        std::cout << "BotDetect = " << input2;
        std::cout << "\n";
        std::cout << "Clear = " << input3;
        std::cout << "\n";
        std::cout << "Enter = " << input4;
        std::cout << "\n";
        std::cout << "Leave = " << input5;
        std::cout << "\n";
        input1 = 0;
        input2 = 0;
        input3 = 0;
        input4 = 0;
        input5 = 0;
    }

    return 0;
}
    
