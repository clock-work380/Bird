#pragma once
// =============================================================================
//  Assets.h  -  SFML3 compatible. sf::Sound requires a SoundBuffer at init.
// =============================================================================
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <array>
#include <iostream>
#include <optional>

namespace Evo {

class Assets {
public:
    // Textures
    sf::Texture bg, base, pipe, birdDown, birdMid, birdUp;
    sf::Texture gameover, message;
    std::array<sf::Texture,10> digits;

    // Sound buffers
    sf::SoundBuffer bufDie, bufHit, bufPoint, bufSwoosh, bufWing;

    // Sounds (constructed after buffers are loaded)
    std::optional<sf::Sound> sndDie, sndHit, sndPoint, sndSwoosh, sndWing;

    bool load(const std::string& dir) {
        bool ok = true;

        auto tex = [&](sf::Texture& t, const std::string& f) {
            if (!t.loadFromFile(dir+f)) {
                std::cerr << "Missing: " << dir+f << "\n"; ok=false;
            } else {
                t.setSmooth(false);
            }
        };

        tex(bg,       "background-day.png");
        tex(base,     "base.png");
        tex(pipe,     "pipe-green.png");
        tex(birdDown, "yellowbird-downflap.png");
        tex(birdMid,  "yellowbird-midflap.png");
        tex(birdUp,   "yellowbird-upflap.png");
        tex(gameover, "gameover.png");
        tex(message,  "message.png");
        for(int i=0;i<10;i++)
            tex(digits[i], std::to_string(i)+".png");

        // Load buffers first, then construct sounds with them
        auto loadSnd = [&](sf::SoundBuffer& buf, std::optional<sf::Sound>& snd,
                           const std::string& f) {
            if (buf.loadFromFile(dir+f))
                snd.emplace(buf);
            else
                std::cerr << "Missing audio: " << dir+f << "\n";
        };

        loadSnd(bufDie,    sndDie,    "die.wav");
        loadSnd(bufHit,    sndHit,    "hit.wav");
        loadSnd(bufPoint,  sndPoint,  "point.wav");
        loadSnd(bufSwoosh, sndSwoosh, "swoosh.wav");
        loadSnd(bufWing,   sndWing,   "wing.wav");

        return ok;
    }

    void play(std::optional<sf::Sound>& snd) {
        if (snd) snd->play();
    }
};

} // namespace Evo
