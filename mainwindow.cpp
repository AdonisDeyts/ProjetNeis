#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QStandardItemModel>
#include <QLocale>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Charger les données initiales
    loadVehicules();
    loadConducteurs();
    loadClients();
    loadMissions();

    // Ajouter des options à la QComboBox
    ui->couleurComboBox->addItem("Rouge");
    ui->couleurComboBox->addItem("Bleu");
    ui->couleurComboBox->addItem("Vert");
    ui->couleurComboBox->addItem("Jaune");
    ui->couleurComboBox->addItem("Noir");
    ui->couleurComboBox->addItem("Blanc");
    ui->couleurComboBox->addItem("Gris");

    // Connecter les QCheckBox aux fonctions de chargement
    connect(ui->filtreImmatriculationCheckBox, &QCheckBox::stateChanged, this, &MainWindow::loadVehicules);
    connect(ui->filtreDescriptionCheckBox, &QCheckBox::stateChanged, this, &MainWindow::loadVehicules);
    connect(ui->filtreCouleurCheckBox, &QCheckBox::stateChanged, this, &MainWindow::loadVehicules);
    connect(ui->filtreKilometrageCheckBox, &QCheckBox::stateChanged, this, &MainWindow::loadVehicules);
    connect(ui->filtreLatitudeCheckBox, &QCheckBox::stateChanged, this, &MainWindow::loadVehicules);
    connect(ui->filtreLongitudeCheckBox, &QCheckBox::stateChanged, this, &MainWindow::loadVehicules);
    connect(ui->filtreDisponibleCheckBox, &QCheckBox::stateChanged, this, &MainWindow::loadVehicules);
    connect(ui->filtrePrenomConducteurCheckBox, &QCheckBox::stateChanged, this, &MainWindow::loadConducteurs);
    connect(ui->filtreNomConducteurCheckBox, &QCheckBox::stateChanged, this, &MainWindow::loadConducteurs);
    connect(ui->filtreEmailConducteurCheckBox, &QCheckBox::stateChanged, this, &MainWindow::loadConducteurs);
    connect(ui->filtreNomClientCheckBox, &QCheckBox::stateChanged, this, &MainWindow::loadClients);
    connect(ui->filtreAdresseCheckBox, &QCheckBox::stateChanged, this, &MainWindow::loadClients);
    connect(ui->filtreImmatriculationMissionCheckBox, &QCheckBox::stateChanged, this, &MainWindow::loadMissions);
    connect(ui->filtreDeltaDistanceCheckBox, &QCheckBox::stateChanged, this, &MainWindow::loadMissions);
    connect(ui->filtreHorodatageDepartCheckBox, &QCheckBox::stateChanged, this, &MainWindow::loadMissions);
    connect(ui->filtreHorodatageArriveeCheckBox, &QCheckBox::stateChanged, this, &MainWindow::loadMissions);
    connect(ui->filtreLavageCheckBox, &QCheckBox::stateChanged, this, &MainWindow::loadMissions);
    connect(ui->filtrePleinCheckBox, &QCheckBox::stateChanged, this, &MainWindow::loadMissions);

    // Connecter les onglets aux fonctions de chargement
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, [this](int index)
    {
        switch (index)
        {
            case 0: loadVehicules(); break;
            case 1: loadConducteurs(); break;
            case 2: loadClients(); break;
            case 3: loadMissions(); break;
        }
    });
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadVehicules()
{
    QStandardItemModel *model = new QStandardItemModel(this);

    // Vérifier quelles colonnes sont sélectionnées
    QStringList headers;
    if (ui->filtreImmatriculationCheckBox->isChecked()) headers << "Immatriculation";
    if (ui->filtreDescriptionCheckBox->isChecked()) headers << "Description";
    if (ui->filtreCouleurCheckBox->isChecked()) headers << "Couleur";
    if (ui->filtreKilometrageCheckBox->isChecked()) headers << "Kilométrage (Km)";
    if (ui->filtreLatitudeCheckBox->isChecked()) headers << "Latitude";
    if (ui->filtreLongitudeCheckBox->isChecked()) headers << "Longitude";
    if (ui->filtreDisponibleCheckBox->isChecked()) headers << "Disponible";

    model->setHorizontalHeaderLabels(headers);

    for (const auto& vehicule : vehicules)
    {
        QList<QStandardItem*> row;
        if (ui->filtreImmatriculationCheckBox->isChecked()) row.append(new QStandardItem(vehicule.immatriculation));
        if (ui->filtreDescriptionCheckBox->isChecked()) row.append(new QStandardItem(vehicule.description));
        if (ui->filtreCouleurCheckBox->isChecked()) row.append(new QStandardItem(vehicule.couleur));
        if (ui->filtreKilometrageCheckBox->isChecked()) row.append(new QStandardItem(QString::number(vehicule.kilometrage)));
        if (ui->filtreLatitudeCheckBox->isChecked()) row.append(new QStandardItem(vehicule.latitude));
        if (ui->filtreLongitudeCheckBox->isChecked()) row.append(new QStandardItem(vehicule.longitude));
        if (ui->filtreDisponibleCheckBox->isChecked()) row.append(new QStandardItem(vehicule.disponible ? "Oui" : "Non"));

        model->appendRow(row);
    }

    ui->vehiculesTableView->setModel(model);
    ui->vehiculesTableView->resizeColumnsToContents();
    ui->vehiculesTableView->resizeRowsToContents();
}


void MainWindow::loadConducteurs()
{
    QStandardItemModel *model = new QStandardItemModel(this);

    QStringList headers;
    if (ui->filtrePrenomConducteurCheckBox->isChecked()) headers << "Prénom";
    if (ui->filtreNomConducteurCheckBox->isChecked()) headers << "Nom";
    if (ui->filtreEmailConducteurCheckBox->isChecked()) headers << "Email";

    model->setHorizontalHeaderLabels(headers);

    for (const auto& conducteur : conducteurs)
    {
        QList<QStandardItem*> row;
        if (ui->filtrePrenomConducteurCheckBox->isChecked()) row.append(new QStandardItem(conducteur.prenom));
        if (ui->filtreNomConducteurCheckBox->isChecked()) row.append(new QStandardItem(conducteur.nom));
        if (ui->filtreEmailConducteurCheckBox->isChecked()) row.append(new QStandardItem(conducteur.email));

        model->appendRow(row);
    }

    ui->conducteursTableView->setModel(model);
    ui->conducteursTableView->resizeColumnsToContents();
    ui->conducteursTableView->resizeRowsToContents();
}

void MainWindow::loadClients()
{
    QStandardItemModel *model = new QStandardItemModel(this);

    QStringList headers;
    if (ui->filtreNomClientCheckBox->isChecked()) headers << "Nom";
    if (ui->filtreAdresseCheckBox->isChecked()) headers << "Adresse Chantier";

    model->setHorizontalHeaderLabels(headers);

    for (const auto& client : clients)
    {
        QList<QStandardItem*> row;
        if (ui->filtreNomClientCheckBox->isChecked()) row.append(new QStandardItem(client.nom));
        if (ui->filtreAdresseCheckBox->isChecked())row.append(new QStandardItem(client.adresseChantier));
        model->appendRow(row);
    }

    ui->clientsTableView->setModel(model);
    ui->clientsTableView->resizeColumnsToContents();
    ui->clientsTableView->resizeRowsToContents();
}

void MainWindow::loadMissions()
{
    QStandardItemModel *model = new QStandardItemModel(this);

    QLocale locale(QLocale::French, QLocale::France);

    QStringList headers;
    if (ui->filtreImmatriculationMissionCheckBox->isChecked()) headers << "Immatriculation";
    if (ui->filtreDeltaDistanceCheckBox->isChecked()) headers << "Delta Distance";
    if (ui->filtreHorodatageDepartCheckBox->isChecked()) headers << "Horodatage Départ";
    if (ui->filtreHorodatageArriveeCheckBox->isChecked()) headers << "Horodatage Arrivée";
    if (ui->filtreLavageCheckBox->isChecked()) headers << "Lavage";
    if (ui->filtrePleinCheckBox->isChecked()) headers << "Plein";

    model->setHorizontalHeaderLabels(headers);

    for (const auto& mission : missions)
    {
        QList<QStandardItem*> row;
        if (ui->filtreImmatriculationMissionCheckBox->isChecked()) row.append(new QStandardItem(mission.immatriculation));
        if (ui->filtreDeltaDistanceCheckBox->isChecked()) row.append(new QStandardItem(mission.deltaDistance));
        if (ui->filtreHorodatageDepartCheckBox->isChecked()) row.append(new QStandardItem(locale.toString(mission.horodatageDepart, "dd MMMM yyyy hh:mm")));
        if (ui->filtreHorodatageArriveeCheckBox->isChecked()) row.append(new QStandardItem(locale.toString(mission.horodatageArrivee, "dd MMMM yyyy hh:mm")));
        if (ui->filtreLavageCheckBox->isChecked()) row.append(new QStandardItem(mission.lavage ? "Fait" : "Non fait"));
        if (ui->filtrePleinCheckBox->isChecked()) row.append(new QStandardItem(mission.plein ? "Fait" : "Non fait"));

        model->appendRow(row);
    }

    ui->missionsTableView->setModel(model);
    ui->missionsTableView->resizeColumnsToContents();
    ui->missionsTableView->resizeRowsToContents();
}


void MainWindow::on_addVehiculeButton_clicked()
{
    Vehicule vehicule;
    vehicule.immatriculation = ui->immatriculationLineEdit->text();
    vehicule.description = ui->descriptionTextEdit->toPlainText();
    vehicule.couleur = ui->couleurComboBox->currentText();
    vehicule.kilometrage = ui->kilometrageSpinBox->value();
    vehicule.latitude = ui->latitudeLineEdit->text();
    vehicule.longitude = ui->longitudeLineEdit->text();
    vehicule.disponible = ui->disponibilitecheckBox->isChecked();

    vehicules.append(vehicule);
    loadVehicules();
}

void MainWindow::on_addConducteurButton_clicked()
{
    Conducteur conducteur;
    conducteur.prenom = ui->prenomLineEdit->text();
    conducteur.nom = ui->nomLineEdit->text();
    conducteur.email = ui->emailLineEdit->text();

    conducteurs.append(conducteur);
    loadConducteurs();
}

void MainWindow::on_addClientButton_clicked()
{
    Client client;
    client.nom = ui->nomClientLineEdit->text();
    client.adresseChantier = ui->adresseChantierLineEdit->text();

    clients.append(client);
    loadClients();
}

void MainWindow::on_addMissionButton_clicked()
{
    Mission mission;
    mission.immatriculation = ui->immatriculationMissionsLineEdit->text();
    mission.deltaDistance = ui->deltaDistanceLineEdit->text();
    mission.horodatageDepart = ui->horodatageDepartEdit->dateTime();
    mission.horodatageArrivee = ui->horodatageArriveeEdit->dateTime();
    mission.lavage = ui->lavageCheckBox->isChecked();
    mission.plein = ui->pleinCheckBox->isChecked();

    missions.append(mission);
    loadMissions();
}

void MainWindow::on_deleteVehiculeButton_clicked()
{
    QItemSelectionModel *select = ui->vehiculesTableView->selectionModel();
    if (select->hasSelection())
    {
        QModelIndexList selectedRows = select->selectedRows();
        if (!selectedRows.isEmpty())
        {
            // Supprimer le véhicule de la liste
            int row = selectedRows.first().row();
            vehicules.removeAt(row);
            loadVehicules();
        }
    }
    else
    {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner un véhicule à supprimer.");
    }
}

void MainWindow::on_deleteConducteurButton_clicked()
{
    QItemSelectionModel *select = ui->conducteursTableView->selectionModel();
    if (select->hasSelection())
    {
        QModelIndexList selectedRows = select->selectedRows();
        if (!selectedRows.isEmpty())
        {
            // Supprimer le conducteur de la liste
            int row = selectedRows.first().row();
            conducteurs.removeAt(row);
            loadConducteurs();
        }
    }
    else
    {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner un conducteur à supprimer.");
    }
}

void MainWindow::on_deleteClientButton_clicked()
{
    QItemSelectionModel *select = ui->clientsTableView->selectionModel();
    if (select->hasSelection())
    {
        QModelIndexList selectedRows = select->selectedRows();
        if (!selectedRows.isEmpty())
        {
            // Supprimer le client de la liste
            int row = selectedRows.first().row();
            clients.removeAt(row);
            loadClients();
        }
    }
    else
    {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner un client à supprimer.");
    }
}

void MainWindow::on_deleteMissionButton_clicked()
{
    QItemSelectionModel *select = ui->missionsTableView->selectionModel();
    if (select->hasSelection())
    {
        QModelIndexList selectedRows = select->selectedRows();
        if (!selectedRows.isEmpty())
        {
            int row = selectedRows.first().row();
            missions.removeAt(row);
            loadMissions();
        }
    }
    else
    {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner une mission à supprimer.");
    }
}

void MainWindow::on_editVehiculeButton_clicked()
{
    QItemSelectionModel *select = ui->vehiculesTableView->selectionModel();
    if (select->hasSelection())
    {
        QModelIndexList selectedRows = select->selectedRows();
        if (!selectedRows.isEmpty())
        {
            int row = selectedRows.first().row();
            Vehicule &vehicule = vehicules[row];

            // Mettre à jour le véhicule avec les nouvelles valeurs
            vehicule.immatriculation = ui->immatriculationLineEdit->text();
            vehicule.description = ui->descriptionTextEdit->toPlainText();
            vehicule.couleur = ui->couleurComboBox->currentText();
            vehicule.kilometrage = ui->kilometrageSpinBox->value();
            vehicule.latitude = ui->latitudeLineEdit->text();
            vehicule.longitude = ui->longitudeLineEdit->text();
            vehicule.disponible = ui->disponibilitecheckBox->isChecked();

            loadVehicules();
        }
    }
    else
    {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner un véhicule à modifier.");
    }
}

void MainWindow::on_editConducteurButton_clicked()
{
    QItemSelectionModel *select = ui->conducteursTableView->selectionModel();
    if (select->hasSelection())
    {
        QModelIndexList selectedRows = select->selectedRows();
        if (!selectedRows.isEmpty())
        {
            int row = selectedRows.first().row();
            Conducteur &conducteur = conducteurs[row];

            // Mettre à jour le conducteur avec les nouvelles valeurs
            conducteur.prenom = ui->prenomLineEdit->text();
            conducteur.nom = ui->nomLineEdit->text();
            conducteur.email = ui->emailLineEdit->text();

            loadConducteurs();
        }
    }
    else
    {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner un conducteur à modifier.");
    }
}

void MainWindow::on_editClientButton_clicked()
{
    QItemSelectionModel *select = ui->clientsTableView->selectionModel();
    if (select->hasSelection())
    {
        QModelIndexList selectedRows = select->selectedRows();
        if (!selectedRows.isEmpty())
        {
            int row = selectedRows.first().row();
            Client &client = clients[row];

            // Mettre à jour le client avec les nouvelles valeurs
            client.nom = ui->nomClientLineEdit->text();
            client.adresseChantier = ui->adresseChantierLineEdit->text();

            loadClients();
        }
    }
    else
    {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner un client à modifier");
    }
}

void MainWindow::on_editMissionButton_clicked()
{
    QItemSelectionModel *select = ui->missionsTableView->selectionModel();
    if (select->hasSelection())
    {
        QModelIndexList selectedRows = select->selectedRows();
        if (!selectedRows.isEmpty())
        {
            int row = selectedRows.first().row();
            Mission &mission = missions[row];
            mission.immatriculation = ui->immatriculationMissionsLineEdit->text();
            mission.deltaDistance = ui->deltaDistanceLineEdit->text();
            mission.horodatageDepart = ui->horodatageDepartEdit->dateTime();
            mission.horodatageArrivee = ui->horodatageArriveeEdit->dateTime();
            mission.lavage = ui->lavageCheckBox->isChecked();
            mission.plein = ui->pleinCheckBox->isChecked();

            loadMissions();
        }
    }
    else
    {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner une mission à modifier");
    }
}
