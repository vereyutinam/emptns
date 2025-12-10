#include "HotelClasses.h"

Room::Room() : Type(""), Cost(0), Discount(0) {}

Room::Room(String^ type, double cost, int discount) : Type(type), Cost(cost), Discount(discount) {}

Hotel::Hotel()
{
    rooms = gcnew List<Room^>();
}

void Hotel::AddRoom(Room^ room)
{
    rooms->Add(room);
}

void Hotel::RemoveRoom(int index)
{
    if (index >= 0 && index < rooms->Count)
    {
        rooms->RemoveAt(index);
    }
}

void Hotel::UpdateRoom(int index, Room^ newRoom)
{
    if (index >= 0 && index < rooms->Count)
    {
        rooms[index] = newRoom;
    }
}

void Hotel::ClearRooms()
{
    rooms->Clear();
}

double Hotel::GetAverageCost()
{
    if (rooms->Count == 0) return 0.0;

    double totalCost = 0;
    for each (Room ^ room in rooms)
    {
        totalCost += room->Cost;
    }
    return totalCost / rooms->Count;
}

List<Room^>^ Hotel::GetRooms()
{
    return rooms;
}

int Hotel::GetRoomsCount()
{
    return rooms->Count;
}
