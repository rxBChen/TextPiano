/*************************************************
 * Public Constants
 *************************************************/
enum Notes
{
  NOTE_C = 262,
  NOTE_D = 294,
  NOTE_E = 330,
  NOTE_F = 349,
  NOTE_G = 392,
  NOTE_A = 440,
  NOTE_B = 494
};

char sendNote(int Note)
{
  switch(Note)
  {
        case NOTE_A:
        return 'A';
        break;
        case NOTE_B:
        return 'B';
        break;
        case NOTE_C:
        return 'C';
        break;
        case NOTE_D:
        return 'D';
        break;
        case NOTE_E:
        return 'E';
        break;
        case NOTE_F:
        return 'F';
        break;
        case NOTE_G:
        return 'G';
        break;
  }
}



