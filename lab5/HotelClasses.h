#pragma once

using namespace System;
using namespace System::Collections::Generic;

public ref class Room
{
public:
    String^ Type;
    double Cost;
    int Discount;

    Room();
    Room(String^ type, double cost, int discount);
};

public ref class Hotel
{
private:
    List<Room^>^ rooms;

public:
    Hotel();
    void AddRoom(Room^ room);
    void RemoveRoom(int index);
    void UpdateRoom(int index, Room^ newRoom);
    void ClearRooms();
    double GetAverageCost();
    List<Room^>^ GetRooms();
    int GetRoomsCount();
};
