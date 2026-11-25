/* File: saeromotique.cpp
 * Author:
 * Source: https://github.com/platu/libsensehat-cpp
 *
 * Patron du code source des séances de travaux pratiques
 */

#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <sensehat.h>

using namespace std;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono;      // system_clock, seconds, milliseconds

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Début constantes

// Fin constantes
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Début sous-programmes
void initPwm(unsigned int channel)
{ // initialize one pwm
    pwmInit(channel);
    pwmPeriod(channel, 10000);
    pwmDutyCycle(channel, 0);
    pwmEnable(channel);
}

void init()
{                          // initialize sensors + each pwm
    gpioSetConfig(16, in); // 16 -> gauche
    gpioSetConfig(17, in); // 17 -> droite
    initPwm(0);
    initPwm(1);
}

void avancer(unsigned int vitesse)
{
    pwmDutyCycle(0, vitesse);
    pwmDutyCycle(1, vitesse); // both pwm at 'vitesse' variable
}

void droite(unsigned int vitesse)
{
    pwmDutyCycle(0, 0);      // shutdown left pwm
    pwmDutyCycle(1, vitesse); // right pwm at 'vitesse' variable
}

void gauche(unsigned int vitesse)
{
    pwmDutyCycle(0, vitesse); // left pwm at 'vitesse' variable
    pwmDutyCycle(1, 0);      // shutdown right pwm
}

void arreter()
{
    pwmDisable(0); // disable 0 (left)
    pwmDisable(1); // disable 1 (right)
}

// Fin sous-programmes
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main()
{
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Début variables
    bool stop = false;
    int capgauche,capdroite;
    // Fin variables
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    if (senseInit())
    {
        cout << "Sense Hat initialization Ok." << endl;
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // Début instructions
        init();
        do
        {
            capgauche=gpioGetInput(16);
            capdroite=gpioGetInput(17);
            //cout << capgauche << " "<< capdroite << "\t" << endl;  //verif a faire
            if (capgauche == 1 && capdroite == 0)
            {
                gauche(65); // left at 65%
            }
            else if (capgauche == 0 && capdroite == 1)
            {
                droite(65); // right at 65%
            }
            else if (capgauche == 1 && capdroite == 1)
            {
                avancer(65); // forward 65%
            }
            else if (capgauche == 0 && capdroite == 0)
            {
                arreter();
                stop = true;
            }
        } while (stop == false);
        // Fin instructions
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        cout << "Press joystick button to quit." << endl;
        senseWaitForJoystickEnter();
        arreter();
        senseShutdown();
        cout << "Sense Hat shut down." << endl;
    }

    return EXIT_SUCCESS;
}