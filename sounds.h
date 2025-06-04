enum SoundType
{
    DEATH = 0,
    BONUS = 1,
    WALL_HIT = 2
};

bool initialize_sounds();
void play_music();
void stop_music();
void play_sfx(SoundType sound_type);