#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Data;
using namespace System::Drawing;

MainForm::MainForm(void)
{
    InitializeComponent();
    hotel = gcnew Hotel();

    // Инициализация БД
    dbManager = gcnew DatabaseManager("hotel.db");

    // Загрузка данных из БД при старте
    LoadDataFromDatabase();
    UpdateStatistics();
}

MainForm::~MainForm()
{
    if (components) delete components;
    if (hotel) delete hotel;
    if (dbManager) delete dbManager;
}

void MainForm::InitializeComponent(void)
{
    this->dataGridViewRooms = gcnew DataGridView();
    this->buttonAdd = gcnew Button();
    this->buttonEdit = gcnew Button();
    this->buttonDelete = gcnew Button();
    this->buttonExport = gcnew Button();
    this->buttonImport = gcnew Button();
    this->saveFileDialog = gcnew SaveFileDialog();
    this->openFileDialog = gcnew OpenFileDialog();
    this->labelStats = gcnew Label();
    this->components = gcnew System::ComponentModel::Container();

    // DataGridView
    this->dataGridViewRooms->Location = Point(12, 12);
    this->dataGridViewRooms->Size = Drawing::Size(760, 300);
    this->dataGridViewRooms->ReadOnly = true;
    this->dataGridViewRooms->AllowUserToAddRows = false;
    this->dataGridViewRooms->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
    this->dataGridViewRooms->AllowUserToOrderColumns = true;

    // Диалоговые окна
    saveFileDialog->Filter = "Text files (*.txt)|*.txt|CSV files (*.csv)|*.csv";
    saveFileDialog->DefaultExt = "txt";
    saveFileDialog->Title = "Экспорт данных из БД";

    openFileDialog->Filter = "Text files (*.txt)|*.txt|CSV files (*.csv)|*.csv";
    openFileDialog->DefaultExt = "txt";
    openFileDialog->Title = "Импорт данных в БД";

    // Кнопки (5 штук)
    int buttonY = 320;
    int buttonWidth = 110;
    int buttonHeight = 30;
    int buttonSpacing = 10;
    int startX = 12;

    this->buttonAdd->Location = Point(startX, buttonY);
    this->buttonAdd->Size = Drawing::Size(buttonWidth, buttonHeight);
    this->buttonAdd->Text = "Добавить";
    this->buttonAdd->Click += gcnew EventHandler(this, &MainForm::buttonAdd_Click);

    this->buttonEdit->Location = Point(startX + (buttonWidth + buttonSpacing), buttonY);
    this->buttonEdit->Size = Drawing::Size(buttonWidth, buttonHeight);
    this->buttonEdit->Text = "Изменить";
    this->buttonEdit->Click += gcnew EventHandler(this, &MainForm::buttonEdit_Click);

    this->buttonDelete->Location = Point(startX + 2 * (buttonWidth + buttonSpacing), buttonY);
    this->buttonDelete->Size = Drawing::Size(buttonWidth, buttonHeight);
    this->buttonDelete->Text = "Удалить";
    this->buttonDelete->Click += gcnew EventHandler(this, &MainForm::buttonDelete_Click);

    this->buttonExport->Location = Point(startX + 3 * (buttonWidth + buttonSpacing), buttonY);
    this->buttonExport->Size = Drawing::Size(buttonWidth, buttonHeight);
    this->buttonExport->Text = "Сохранить";
    this->buttonExport->Click += gcnew EventHandler(this, &MainForm::buttonExport_Click);

    this->buttonImport->Location = Point(startX + 4 * (buttonWidth + buttonSpacing), buttonY);
    this->buttonImport->Size = Drawing::Size(buttonWidth, buttonHeight);
    this->buttonImport->Text = "Загрузить";
    this->buttonImport->Click += gcnew EventHandler(this, &MainForm::buttonImport_Click);

    // Label статистики
    this->labelStats->Location = Point(12, 360);
    this->labelStats->Size = Drawing::Size(500, 20);
    this->labelStats->Text = "Статистика:";

    // Форма
    this->Text = "ProjectLab5 - Управление гостиницей (БД)";
    this->Size = Drawing::Size(800, 450);

    // Добавляем элементы
    this->Controls->Add(dataGridViewRooms);
    this->Controls->Add(buttonAdd);
    this->Controls->Add(buttonEdit);
    this->Controls->Add(buttonDelete);
    this->Controls->Add(buttonExport);
    this->Controls->Add(buttonImport);
    this->Controls->Add(labelStats);
}

void MainForm::LoadDataFromDatabase()
{
    // Очищаем Hotel
    hotel->ClearRooms();

    // Загружаем из БД
    auto roomsFromDB = dbManager->GetAllRooms();

    // Добавляем в Hotel
    for each (Room ^ room in roomsFromDB)
    {
        hotel->AddRoom(room);
    }

    // Обновляем отображение
    UpdateRoomsDisplay();
}

void MainForm::SaveDataToDatabase()
{
    // Сохраняем все комнаты из Hotel в БД
    dbManager->ClearAllRooms();

    auto rooms = hotel->GetRooms();
    for each (Room ^ room in rooms)
    {
        dbManager->AddRoom(room);
    }
}

void MainForm::buttonAdd_Click(System::Object^ sender, System::EventArgs^ e)
{
    AddEditForm^ form = gcnew AddEditForm();
    if (form->ShowDialog() == Windows::Forms::DialogResult::OK)
    {
        Room^ newRoom = form->GetRoom;

        // Добавляем в БД
        if (dbManager->AddRoom(newRoom))
        {
            // Добавляем в Hotel
            hotel->AddRoom(newRoom);

            // Обновляем интерфейс
            UpdateRoomsDisplay();
            UpdateStatistics();
        }
        else
        {
            MessageBox::Show("Ошибка при добавлении в базу данных!", "Ошибка");
        }
    }
}

void MainForm::buttonEdit_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (dataGridViewRooms->SelectedRows->Count == 0)
    {
        MessageBox::Show("Выберите номер для редактирования!", "Ошибка");
        return;
    }

    int index = dataGridViewRooms->SelectedRows[0]->Index;
    auto rooms = hotel->GetRooms();

    if (index >= 0 && index < rooms->Count)
    {
        AddEditForm^ form = gcnew AddEditForm(rooms[index], index);
        if (form->ShowDialog() == Windows::Forms::DialogResult::OK)
        {
            Room^ updatedRoom = form->GetRoom;

            // Получаем ID из БД
            int roomId = dbManager->GetRoomId(index);
            if (roomId != -1)
            {
                // Обновляем в БД
                if (dbManager->UpdateRoom(roomId, updatedRoom))
                {
                    // Обновляем в Hotel
                    hotel->UpdateRoom(index, updatedRoom);

                    // Обновляем интерфейс
                    UpdateRoomsDisplay();
                    UpdateStatistics();
                }
                else
                {
                    MessageBox::Show("Ошибка при обновлении в БД!", "Ошибка");
                }
            }
        }
    }
}

void MainForm::buttonDelete_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (dataGridViewRooms->SelectedRows->Count == 0)
    {
        MessageBox::Show("Выберите номер для удаления!", "Ошибка");
        return;
    }

    int index = dataGridViewRooms->SelectedRows[0]->Index;

    // Получаем ID из БД
    int roomId = dbManager->GetRoomId(index);
    if (roomId != -1)
    {
        // Удаляем из БД
        if (dbManager->DeleteRoom(roomId))
        {
            // Удаляем из Hotel
            hotel->RemoveRoom(index);

            // Обновляем интерфейс
            UpdateRoomsDisplay();
            UpdateStatistics();
        }
        else
        {
            MessageBox::Show("Ошибка при удалении из БД!", "Ошибка");
        }
    }
}

void MainForm::buttonExport_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (saveFileDialog->ShowDialog() == Windows::Forms::DialogResult::OK)
    {
        System::String^ filePath = saveFileDialog->FileName;
        if (dbManager->ExportToFile(filePath))
        {
            MessageBox::Show("Данные успешно экспортированы из БД в файл!", "Успех");
        }
        else
        {
            MessageBox::Show("Ошибка при экспорте данных!", "Ошибка");
        }
    }
}

void MainForm::buttonImport_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (openFileDialog->ShowDialog() == Windows::Forms::DialogResult::OK)
    {
        System::String^ filePath = openFileDialog->FileName;
        if (dbManager->ImportFromFile(filePath))
        {
            // Перезагружаем данные из БД
            LoadDataFromDatabase();
            MessageBox::Show("Данные успешно импортированы из файла в БД!", "Успех");
        }
        else
        {
            MessageBox::Show("Ошибка при импорте данных!", "Ошибка");
        }
    }
}

void MainForm::UpdateRoomsDisplay()
{
    dataGridViewRooms->Rows->Clear();
    dataGridViewRooms->Columns->Clear();

    // Добавляем колонки
    dataGridViewRooms->Columns->Add("Number", "№");
    dataGridViewRooms->Columns->Add("Type", "Тип");
    dataGridViewRooms->Columns->Add("Cost", "Стоимость");
    dataGridViewRooms->Columns->Add("Discount", "Скидка %");

    auto rooms = hotel->GetRooms();
    for (int i = 0; i < rooms->Count; i++)
    {
        dataGridViewRooms->Rows->Add(
            (i + 1).ToString(),
            rooms[i]->Type,
            rooms[i]->Cost.ToString("F2"),
            rooms[i]->Discount.ToString()
        );
    }
}

void MainForm::UpdateStatistics()
{
    double avgCost = hotel->GetAverageCost();
    int count = hotel->GetRoomsCount();
    labelStats->Text = String::Format(
        "Количество номеров: {0} | Средняя стоимость: {1:F2}",
        count, avgCost
    );
}
