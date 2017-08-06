#ifndef SCENEINFOTRANS_H
#define SCENEINFOTRANS_H

#include<QGraphicsScene>

class sceneinfotrans : public QGraphicsScene
{
public:
    sceneinfotrans();

public slots:
    void scenetobeset(bool a);

signals:
    void toshowgrid(sceneinfotrans &b);

};

#endif // SCENEINFOTRANS_H
