// DatabaseManager.cpp
#include "DatabaseManager.h"
#include "sqlite3.h"  // Включаем напрямую
#include <string>

DatabaseManager::DatabaseManager(System::String^ path) : db(nullptr)
{
    dbPath = path;
    InitializeDatabase();
}

DatabaseManager::~DatabaseManager()
{
    if (db)
    {
        sqlite3_close((sqlite3*)db);
        db = nullptr;
    }
}

void DatabaseManager::InitializeDatabase()
{
    // Преобразуем String^ в std::string
    std::string nativePath;
    for (int i = 0; i < dbPath->Length; i++)
    {
        nativePath += (char)dbPath[i];
    }

    sqlite3* database = nullptr;
    int rc = sqlite3_open(nativePath.c_str(), &database);

    if (rc == SQLITE_OK)
    {
        db = (void*)database;

        const char* sql =
            "CREATE TABLE IF NOT EXISTS Rooms ("
            "Id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "Type TEXT NOT NULL,"
            "Cost REAL NOT NULL,"
            "Discount INTEGER NOT NULL);";

        sqlite3_exec((sqlite3*)db, sql, nullptr, nullptr, nullptr);
    }
}

bool DatabaseManager::IsConnected()
{
    return db != nullptr;
}

bool DatabaseManager::AddRoom(Room^ room)
{
    if (!IsConnected()) return false;

    const char* sql = "INSERT INTO Rooms (Type, Cost, Discount) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2((sqlite3*)db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    // Преобразуем String^ в std::string
    std::string typeStr;
    for (int i = 0; i < room->Type->Length; i++)
    {
        typeStr += (char)room->Type[i];
    }

    sqlite3_bind_text(stmt, 1, typeStr.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 2, room->Cost);
    sqlite3_bind_int(stmt, 3, room->Discount);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

bool DatabaseManager::UpdateRoom(int id, Room^ room)
{
    if (!IsConnected()) return false;

    const char* sql = "UPDATE Rooms SET Type = ?, Cost = ?, Discount = ? WHERE Id = ?;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2((sqlite3*)db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    std::string typeStr;
    for (int i = 0; i < room->Type->Length; i++)
    {
        typeStr += (char)room->Type[i];
    }

    sqlite3_bind_text(stmt, 1, typeStr.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 2, room->Cost);
    sqlite3_bind_int(stmt, 3, room->Discount);
    sqlite3_bind_int(stmt, 4, id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

bool DatabaseManager::DeleteRoom(int id)
{
    if (!IsConnected()) return false;

    const char* sql = "DELETE FROM Rooms WHERE Id = ?;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2((sqlite3*)db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_int(stmt, 1, id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

System::Collections::Generic::List<Room^>^ DatabaseManager::GetAllRooms()
{
    auto rooms = gcnew System::Collections::Generic::List<Room^>();

    if (!IsConnected()) return rooms;

    const char* sql = "SELECT Id, Type, Cost, Discount FROM Rooms ORDER BY Cost;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2((sqlite3*)db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return rooms;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const unsigned char* typeText = sqlite3_column_text(stmt, 1);
        double cost = sqlite3_column_double(stmt, 2);
        int discount = sqlite3_column_int(stmt, 3);

        if (typeText)
        {
            const char* cstr = (const char*)typeText;
            System::String^ typeStr = gcnew System::String(cstr);
            rooms->Add(gcnew Room(typeStr, cost, discount));
        }
    }

    sqlite3_finalize(stmt);
    return rooms;
}

int DatabaseManager::GetRoomId(int index)
{
    if (!IsConnected()) return -1;

    const char* sql = "SELECT Id FROM Rooms ORDER BY Cost;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2((sqlite3*)db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return -1;

    int current = 0;
    int id = -1;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        if (current == index)
        {
            id = sqlite3_column_int(stmt, 0);
            break;
        }
        current++;
    }

    sqlite3_finalize(stmt);
    return id;
}

void DatabaseManager::ClearAllRooms()
{
    if (!IsConnected()) return;

    const char* sql = "DELETE FROM Rooms;";
    sqlite3_exec((sqlite3*)db, sql, nullptr, nullptr, nullptr);
}

bool DatabaseManager::ExportToFile(System::String^ filePath)
{
    try
    {
        // Используем StreamWriter для простоты
        System::IO::StreamWriter^ file = gcnew System::IO::StreamWriter(filePath);

        auto rooms = GetAllRooms();

        for each (Room ^ room in rooms)
        {
            file->WriteLine("{0},{1},{2}",
                room->Type,
                room->Cost.ToString("F2"),
                room->Discount.ToString());
        }

        file->Close();
        return true;
    }
    catch (System::Exception^)
    {
        return false;
    }
}

bool DatabaseManager::ImportFromFile(System::String^ filePath)
{
    try
    {
        ClearAllRooms();

        System::IO::StreamReader^ file = gcnew System::IO::StreamReader(filePath);
        System::String^ line;

        while ((line = file->ReadLine()) != nullptr)
        {
            if (line->Trim()->Length == 0) continue;

            array<System::String^>^ parts = line->Split(',');
            if (parts->Length >= 3)
            {
                System::String^ type = parts[0]->Trim();
                double cost = System::Convert::ToDouble(parts[1]->Trim());
                int discount = System::Convert::ToInt32(parts[2]->Trim());

                Room^ room = gcnew Room(type, cost, discount);
                AddRoom(room);
            }
        }

        file->Close();
        return true;
    }
    catch (System::Exception^)
    {
        return false;
    }
}
