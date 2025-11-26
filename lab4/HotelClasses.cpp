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

void Hotel::SortByCost()
{
    for (int i = 0; i < rooms->Count - 1; i++)
    {
        for (int j = i + 1; j < rooms->Count; j++)
        {
            if (rooms[i]->Cost > rooms[j]->Cost)
            {
                Room^ temp = rooms[i];
                rooms[i] = rooms[j];
                rooms[j] = temp;
            }
        }
    }
}

void Hotel::SaveToFile(String^ filename)
{
    StreamWriter^ file = gcnew StreamWriter(filename);
    for each (Room ^ room in rooms)
    {
        file->WriteLine("{0},{1},{2}", room->Type, room->Cost, room->Discount);
    }
    file->Close();
}

void Hotel::LoadFromFile(String^ filename)
{
    rooms->Clear();
    StreamReader^ file = gcnew StreamReader(filename);
    String^ line;
    while ((line = file->ReadLine()) != nullptr)
    {
        array<String^>^ parts = line->Split(',');
        if (parts->Length == 3)
        {
            String^ type = parts[0];
            double cost = System::Convert::ToDouble(parts[1]);
            int discount = System::Convert::ToInt32(parts[2]);
            rooms->Add(gcnew Room(type, cost, discount));
        }
    }
    file->Close();
}

int Hotel::GetRoomsCount()
{
    return rooms->Count;
}
