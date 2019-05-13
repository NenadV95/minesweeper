#ifndef FIELD_HPP
#define FIELD_HPP


class Field
{
public:
    Field();
    ~Field();
    bool getOpened();
    void setOpened(bool o);
    bool getFlagged();
    void setFlagged(bool f);
    bool getHasMine();
    void setHasMine(bool hm);
    int getNumberOfMines();
    void setNumberOfMines(int nom);

private:
    bool opened;
    bool flagged;
    bool hasMine;
    int numberOfMines;

};

#endif // FIELD_HPP
