#pragma once
#include "HotelClasses.h"

// Объявление структур SQLite
struct sqlite3;
struct sqlite3_stmt;

public ref class DatabaseManager
{
private:
    void* db;  // Нативный указатель
    System::String^ dbPath;

    void InitializeDatabase();

public:
    DatabaseManager(System::String^ path);
    ~DatabaseManager();

    bool IsConnected();
    bool AddRoom(Room^ room);
    bool UpdateRoom(int id, Room^ room);
    bool DeleteRoom(int id);
    System::Collections::Generic::List<Room^>^ GetAllRooms();
    bool ExportToFile(System::String^ filePath);
    bool ImportFromFile(System::String^ filePath);
    void ClearAllRooms();
    int GetRoomId(int index);
};
