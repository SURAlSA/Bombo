#include "Bombo.h"

// variables

// functions

void Bombo::greeting()
{
    UserManager UserManager;
    string greetings[] = {"Hi ", "Hello ", "こんにちは ", "你好 "};
    string emoticons[] = {" :)", " :>", " :|", " (˶ᵔ ᵕ ᵔ˶)", " >ᴗ<", " (ᵕ—ᴗ—)", " (,,>﹏<,,)", " (˶˃⤙˂˶)"};
    int index = getRandomInt(0, 3);
    int index2 = getRandomInt(0, 7);
    if (UserManager.UsersEnabled())
    {
        cout << greetings[index] << userName << emoticons[index2] << endl;
    }
    else
    {
        cout << greetings[index] << "whoever you are" << emoticons[index2] << endl;
    }
}
void Bombo::joke()
{
    string animal[] = {"Dog", "Cat", "Chicken", "Pig", "Cow", "Goat", "Crocodile", "Duck", "Bombo"};
    int animalIndex = getRandomInt(0, 8);
    string question[] = {"cross the road?", "chase the chicken?", "order lemonade?", "get water?", "forget its homework?", "forget its birthday?", "eat a cake?", "fight a kangaroo?",
    "fight a Bombo?", "commit treason?", "lite fire to a cows tail?"};
    int questionIndex = getRandomInt(0, 10);
    string response[] = {"To eat KFC on the otherside.", "To run from a capybara.", "Because it felt like it.", "Because it was funny.", "I dont know, do you?", "Because it was hungry.",
    "To get to the other side.", "To get a drink of water.", "Because it was not not not hungry.", "We will never know.", "To cross into the other universe."};
    int responseIndex = getRandomInt(0, 10);

    cout << "Why did the " << animal[animalIndex] << " " << question[questionIndex] << endl;
    cout << response[responseIndex] << endl;
}
