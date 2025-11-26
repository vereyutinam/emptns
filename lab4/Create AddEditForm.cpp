#include "AddEditForm.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

AddEditForm::AddEditForm(void)
{
    InitializeComponent();
    this->Text = "Добавить номер";
    buttonAction->Text = "Добавить";
    isEditing = false;
    roomIndex = -1;
}

AddEditForm::AddEditForm(Room^ room, int index)
{
    InitializeComponent();
    this->Text = "Редактировать номер";
    buttonAction->Text = "Сохранить";
    isEditing = true;
    roomIndex = index;
    currentRoom = room;

    comboBoxType->Text = room->Type;
    textBoxDiscount->Text = room->Discount.ToString();
}

AddEditForm::~AddEditForm()
{
    if (components)
    {
        delete components;
    }
}

void AddEditForm::InitializeComponent(void)
{
    this->comboBoxType = (gcnew System::Windows::Forms::ComboBox());
    this->textBoxDiscount = (gcnew System::Windows::Forms::TextBox());
    this->buttonAction = (gcnew System::Windows::Forms::Button());
    this->buttonCancel = (gcnew System::Windows::Forms::Button());
    this->labelType = (gcnew System::Windows::Forms::Label());
    this->labelDiscount = (gcnew System::Windows::Forms::Label());
    this->SuspendLayout();

    // comboBoxType
    this->comboBoxType->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
    this->comboBoxType->FormattingEnabled = true;
    this->comboBoxType->Items->AddRange(gcnew cli::array< System::Object^  >(3) {
        L"Economy", L"Standard", L"Premium"
    });
    this->comboBoxType->Location = System::Drawing::Point(120, 20);
    this->comboBoxType->Name = L"comboBoxType";
    this->comboBoxType->Size = System::Drawing::Size(150, 24);
    this->comboBoxType->TabIndex = 0;
    this->comboBoxType->SelectedIndex = 0;

    // textBoxDiscount
    this->textBoxDiscount->Location = System::Drawing::Point(120, 60);
    this->textBoxDiscount->Name = L"textBoxDiscount";
    this->textBoxDiscount->Size = System::Drawing::Size(150, 22);
    this->textBoxDiscount->TabIndex = 1;

    // buttonAction
    this->buttonAction->Location = System::Drawing::Point(50, 100);
    this->buttonAction->Name = L"buttonAction";
    this->buttonAction->Size = System::Drawing::Size(100, 30);
    this->buttonAction->TabIndex = 2;
    this->buttonAction->UseVisualStyleBackColor = true;
    this->buttonAction->Click += gcnew System::EventHandler(this, &AddEditForm::buttonAction_Click);

    // buttonCancel
    this->buttonCancel->Location = System::Drawing::Point(170, 100);
    this->buttonCancel->Name = L"buttonCancel";
    this->buttonCancel->Size = System::Drawing::Size(100, 30);
    this->buttonCancel->TabIndex = 3;
    this->buttonCancel->Text = L"Отмена";
    this->buttonCancel->UseVisualStyleBackColor = true;
    this->buttonCancel->Click += gcnew System::EventHandler(this, &AddEditForm::buttonCancel_Click);

    // labelType
    this->labelType->Location = System::Drawing::Point(20, 20);
    this->labelType->Name = L"labelType";
    this->labelType->Size = System::Drawing::Size(100, 20);
    this->labelType->TabIndex = 4;
    this->labelType->Text = L"Тип номера:";

    // labelDiscount
    this->labelDiscount->Location = System::Drawing::Point(20, 60);
    this->labelDiscount->Name = L"labelDiscount";
    this->labelDiscount->Size = System::Drawing::Size(100, 20);
    this->labelDiscount->TabIndex = 5;
    this->labelDiscount->Text = L"Скидка (%):";

    // AddEditForm
    this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
    this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
    this->ClientSize = System::Drawing::Size(320, 150);
    this->Controls->Add(this->labelDiscount);
    this->Controls->Add(this->labelType);
    this->Controls->Add(this->buttonCancel);
    this->Controls->Add(this->buttonAction);
    this->Controls->Add(this->textBoxDiscount);
    this->Controls->Add(this->comboBoxType);
    this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
    this->MaximizeBox = false;
    this->MinimizeBox = false;
    this->Name = L"AddEditForm";
    this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
    this->ResumeLayout(false);
    this->PerformLayout();
}

void AddEditForm::buttonAction_Click(System::Object^ sender, System::EventArgs^ e)
{
    if (comboBoxType->SelectedItem == nullptr)
    {
        MessageBox::Show("Выберите тип номера!", "Ошибка");
        return;
    }

    int discount;
    if (!Int32::TryParse(textBoxDiscount->Text, discount))
    {
        MessageBox::Show("Введите корректное число для скидки!", "Ошибка");
        return;
    }

    if (discount < 0 || discount > 100)
    {
        MessageBox::Show("Скидка должна быть от 0 до 100%!", "Ошибка");
        return;
    }

    String^ type = comboBoxType->SelectedItem->ToString();
    double cost = CalculateCost(type, discount);

    currentRoom = gcnew Room(type, cost, discount);
    this->DialogResult = Windows::Forms::DialogResult::OK;
    this->Close();
}

void AddEditForm::buttonCancel_Click(System::Object^ sender, System::EventArgs^ e)
{
    this->DialogResult = Windows::Forms::DialogResult::Cancel;
    this->Close();
}

double AddEditForm::CalculateCost(String^ type, int discount)
{
    if (type == "Economy") return 12 * (100 - discount);
    if (type == "Standard") return 20 * (100 - discount);
    if (type == "Premium") return 80 * (100 - discount);
    return 0;
}
