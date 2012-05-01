
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include "arduino-serial.c"

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////

static const int image_counter_max = 3;
static const int image_counter_min = 0;
float refresh_rate = 2.0f; // Refresh rate in seconds.

sf::Clock Clock;

int main()
{
    // Open the serial port for reading:
    int fd = serialport_init("/dev/ttyACM0", 115200);
    serialport_writebyte(fd, 'A');
    serialport_writebyte(fd, '\n');
    char *buf;
    while(true)
    {
        int result = serialport_read_until(fd, buf, '');
        printf("%i ", result);
    }

    // Set the last signalled state of the system as sent by the opto signal over USB.
    // "t" is top-dead-center, and "b" is bottom-dead-center.
    char system_state = 'B';
    int image_counter = image_counter_min;    

    // Create the main rendering window
    sf::RenderWindow App(sf::VideoMode(800, 600, 32), "SFML Graphics");

    // Use v-sync (make it so that we sync with the monitor, and don't draw more than 60fps.
    App.UseVerticalSync(false);
    // App.SetFramerateLimit(60); // Limit to 60 frames per second
    // App.SetFramerateLimit(0);  // No limit

    // Start game loop
    while (App.IsOpened())
    {
        // Process events
        sf::Event Event;
        while (App.GetEvent(Event))
        {
            // Close window : exit
            if (Event.Type == sf::Event::Closed)
                App.Close();

            // A key has been pressed
            if (Event.Type == sf::Event::KeyPressed)
            {
                // Escape key : exit
                if (Event.Key.Code == sf::Key::Escape)
                    App.Close();
            }
        }

        // Read from the USB port.
        char *buf;
        int result = serialport_read_until(fd, buf, ' ');
        printf("%i ", result);
        if (result) 
        {
            char input_state = buf[0];
            if (input_state != system_state && (input_state == 'B' || input_state == 'T'))
            {   // Start the appropriate image sequence.

                // Change the system state.
                system_state = input_state;
                //printf("%c\n", system_state);
                
                // Reset the image counter.
                image_counter = 0; 
            }
        }

        // Get the elapsed time.
        float elapsed = Clock.GetElapsedTime();
        if (elapsed > refresh_rate)
        {
            // Update the image.
            printf("%i %f\n", image_counter, elapsed);
            Clock.Reset();

            // Increment or decrement the image counter.
            if (system_state == 'B') image_counter++;
            else if (system_state == 'T') image_counter++;
            
            // Make sure the counter doesn't go out of bounds.
            if (image_counter > image_counter_max) image_counter = image_counter_max;
            if (image_counter < image_counter_min) image_counter = image_counter_min;

            // Draw the appropriate colour:
            switch( image_counter ) 
            {
                case 0:
                    App.Clear(sf::Color(0, 0, 0));
                    break;
                case 1:
                    App.Clear(sf::Color(85, 85, 85));
                    break;
                case 2:
                    App.Clear(sf::Color(170, 170, 170));
                    break;
                case 3:
                    App.Clear(sf::Color(255, 255, 255));
                    break;
            }

        }

        // Clear the screen with red color
        // App.Clear(sf::Color(200, 0, 0));

        // Display window contents on screen
        App.Display();
    }

    return EXIT_SUCCESS;
}
