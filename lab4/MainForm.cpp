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
    UpdateRoomsDisplay();
    UpdateStatistics();
}

MainForm::~MainForm()
{
    if (components) delete components;
    if (hotel) delete hotel;
}

void MainForm::InitializeComponent(void)
{
    this->dataGridViewRooms = gcnew DataGridView();
    this->buttonAdd = gcnew Button();
    this->buttonEdit = gcnew Button();
    this->buttonDelete = gcnew Button();
    this->buttonSort = gcnew Button();
    this->buttonSave = gcnew Button();
    this->buttonLoad = gcnew Button();
    this->labelStats = gcnew Label();
    this->components = gcnew System::ComponentModel::Container();

    // DataGridView
    this->dataGridViewRooms->Location = Point(12, 12);
    this->dataGridViewRooms->Size = Drawing::Size(600, 300);
    this->dataGridViewRooms->ReadOnly = true;
    this->dataGridViewRooms->AllowUserToAddRows = false;
    this->dataGridViewRooms->SelectionMode = DataGridViewSelectionMode::FullRowSelect;

    // Кнопки
    this->buttonAdd->Location = Point(12, 320);
    this->buttonAdd->Size = Drawing::Size(80, 30);
    this->buttonAdd->Text = "Добавить";
    this->buttonAdd->Click += gcnew EventHandler(this, &MainForm::buttonAdd_Click);

    this->buttonEdit->Location = Point(102, 320);
    this->buttonEdit->Size = Drawing::Size(80, 30);
    this->buttonEdit->Text = "Изменить";
    this->buttonEdit->Click += gcnew EventHandler(this, &MainForm::buttonEdit_Click);

    this->buttonDelete->Location = Point(192, 320);
    this->buttonDelete->Size = Drawing::Size(80, 30);
    this->buttonDelete->Text = "Удалить";
    this->buttonDelete->Click += gcnew EventHandler(this, &MainForm::buttonDelete_Click);

    this->buttonSort->Location = Point(282, 320);
    this->buttonSort->Size = Drawing::Size(80, 30);
    this->buttonSort->Text = "Сортировать";
    this->buttonSort->Click += gcnew EventHandler(this, &MainForm::buttonSort_Click);

    this->buttonSave->Location = Point(372, 320);
    this->buttonSave->Size = Drawing::Size(80, 30);
    this->buttonSave->Text = "Сохранить";
    this->buttonSave->Click += gcnew EventHandler(this, &MainForm::buttonSave_Click);

    this->buttonLoad->Location = Point(462, 320);
    this->buttonLoad->Size = Drawing::Size(80, 30);
    this->buttonLoad->Text = "Загрузить";
    this->buttonLoad->Click += gcnew EventHandler(this, &MainForm::buttonLoad_Click);

    // Label статистики
    this->labelStats->Location = Point(12, 360);
    this->labelStats->Size = Drawing::Size(400, 20);
    this->labelStats->Text = "Статистика:";

    // Форма
    this->Text = "ProjectLab4 - Управление гостиницей";
    this->Size = Drawing::Size(650, 450);
    this->Controls->Add(dataGridViewRooms);
    this->Controls->Add(buttonAdd);
    this->Controls->Add(buttonEdit);
    this->Controls->Add(buttonDelete);
    this->Controls->Add(buttonSort);
    this->Controls->Add(buttonSave);
    this->Controls->Add(buttonLoad);
    this->Controls->Add(labelStats);
}

void MainForm::buttonAdd_Click(System::Object^ sender, System::EventArgs^ e)
{
    AddEditForm^ form = gcnew AddEditForm();
    if (form->ShowDialog() == Windows::Forms::DialogResult::OK)
    {
        hotel->AddRoom(form->GetRoom);
        UpdateRoomsDisplay();
        UpdateStatistics();
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
    List<Room^>^ rooms = hotel->GetRooms();
    if (index >= 0 && index < rooms->Count)
    {
        AddEditForm^ form = gcnew AddEditForm(rooms[index], index);
        if (form->ShowDialog() == Windows::Forms::DialogResult::OK)
        {
            hotel->UpdateRoom(index, form->GetRoom);
            UpdateRoomsDisplay();
            UpdateStatistics();
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
    hotel->RemoveRoom(index);
    UpdateRoomsDisplay();
    UpdateStatistics();
}

void MainForm::buttonSort_Click(System::Object^ sender, System::EventArgs^ e)
{
    hotel->SortByCost();
    UpdateRoomsDisplay();
}

void MainForm::buttonSave_Click(System::Object^ sender, System::EventArgs^ e)
{
    SaveFileDialog^ dialog = gcnew SaveFileDialog();
    dialog->Filter = "Text files (*.txt)|*.txt";
    if (dialog->ShowDialog() == Windows::Forms::DialogResult::OK)
    {
        try
        {
            hotel->SaveToFile(dialog->FileName);
            MessageBox::Show("Данные сохранены!", "Успех");
        }
        catch (Exception^ ex)
        {
            MessageBox::Show("Ошибка сохранения: " + ex->Message, "Ошибка");
        }
    }
}

void MainForm::buttonLoad_Click(System::Object^ sender, System::EventArgs^ e)
{
    OpenFileDialog^ dialog = gcnew OpenFileDialog();
    dialog->Filter = "Text files (*.txt)|*.txt";
    if (dialog->ShowDialog() == Windows::Forms::DialogResult::OK)
    {
        try
        {
            hotel->LoadFromFile(dialog->FileName);
            UpdateRoomsDisplay();
            UpdateStatistics();
            MessageBox::Show("Данные загружены!", ":)");
        }
        catch (Exception^ ex)
        {
            MessageBox::Show("Ошибка загрузки: " + ex->Message, "Ошибка");
        }
    }
}

void MainForm::UpdateRoomsDisplay()
{
    dataGridViewRooms->Rows->Clear();
    dataGridViewRooms->Columns->Clear();

    dataGridViewRooms->Columns->Add("Number", "№");
    dataGridViewRooms->Columns->Add("Type", "Тип");
    dataGridViewRooms->Columns->Add("Cost", "Стоимость");
    dataGridViewRooms->Columns->Add("Discount", "Скидка %");

    List<Room^>^ rooms = hotel->GetRooms();
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
