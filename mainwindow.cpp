#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QStandardItemModel>
#include <QLocale>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug() << "Available drivers:" << QSqlDatabase::drivers();

    // Établir la connexion à la base de données
    if (!createConnection())
    {
        QMessageBox::critical(this, "Erreur", "Impossible de se connecter à la base de données.");
        return;
    }

    // Charger les données initiales
    loadVehicules();
    loadConducteurs();
    loadClients();
    loadMissions();

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
    connect(ui->filtreRemboursementLavageCheckBox, &QCheckBox::stateChanged, this, &MainWindow::loadMissions);
    connect(ui->filtreRemboursementPleinCheckBox, &QCheckBox::stateChanged, this, &MainWindow::loadMissions);

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

bool MainWindow::createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("192.168.17.13");
    db.setDatabaseName("projet_neis");
    db.setUserName("serv_admin");
    db.setPassword("serv_admin");

    if (!db.open())
    {
        qDebug() << "Cannot connect to database:" << db.lastError();
        return false;
    }
    qDebug() << "Connected to database successfully.";
    return true;
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

    QSqlQuery query("SELECT * FROM vehicules");
    while (query.next()) {
        Vehicule vehicule;
        vehicule.immatriculation = query.value("immatriculation").toString();
        vehicule.description = query.value("description").toString();
        vehicule.couleur = query.value("couleur").toString();
        vehicule.kilometrage = query.value("kilometrage").toDouble();
        vehicule.latitude = query.value("latitude").toString();
        vehicule.longitude = query.value("longitude").toString();
        vehicule.disponible = query.value("disponible").toBool();
        vehicules.append(vehicule);

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

    QSqlQuery query("SELECT * FROM conducteurs");
    while (query.next()) {
        Conducteur conducteur;
        conducteur.prenom = query.value("prenom").toString();
        conducteur.nom = query.value("nom").toString();
        conducteur.email = query.value("email").toString();
        conducteurs.append(conducteur);

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

    QSqlQuery query("SELECT * FROM clients");
    while (query.next()) {
        Client client;
        client.nom = query.value("nom").toString();
        client.adresseChantier = query.value("adresseChantier").toString();
        clients.append(client);

        QList<QStandardItem*> row;
        if (ui->filtreNomClientCheckBox->isChecked()) row.append(new QStandardItem(client.nom));
        if (ui->filtreAdresseCheckBox->isChecked()) row.append(new QStandardItem(client.adresseChantier));
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
    if (ui->filtreRemboursementLavageCheckBox->isChecked()) headers << "Remboursement Lavage";
    if (ui->filtreRemboursementPleinCheckBox->isChecked()) headers << "Remboursement Plein";

    model->setHorizontalHeaderLabels(headers);

    QSqlQuery query("SELECT * FROM missions");
    while (query.next()) {
        Mission mission;
        mission.immatriculation = query.value("immatriculation").toString();
        mission.deltaDistance = query.value("deltaDistance").toString();
        mission.horodatageDepart = query.value("horodatageDepart").toDateTime();
        mission.horodatageArrivee = query.value("horodatageArrivee").toDateTime();
        mission.lavage = query.value("lavage").toBool();
        mission.plein = query.value("plein").toBool();
        mission.remboursementLavage = query.value("remboursementLavage").toBool();
        mission.remboursementPlein = query.value("remboursementPlein").toBool();
        missions.append(mission);

        QList<QStandardItem*> row;
        if (ui->filtreImmatriculationMissionCheckBox->isChecked()) row.append(new QStandardItem(mission.immatriculation));
        if (ui->filtreDeltaDistanceCheckBox->isChecked()) row.append(new QStandardItem(mission.deltaDistance));
        if (ui->filtreHorodatageDepartCheckBox->isChecked()) row.append(new QStandardItem(locale.toString(mission.horodatageDepart, "dd MMMM yyyy hh:mm")));
        if (ui->filtreHorodatageArriveeCheckBox->isChecked()) row.append(new QStandardItem(locale.toString(mission.horodatageArrivee, "dd MMMM yyyy hh:mm")));
        if (ui->filtreLavageCheckBox->isChecked()) row.append(new QStandardItem(mission.lavage ? "Fait" : "Non fait"));
        if (ui->filtrePleinCheckBox->isChecked()) row.append(new QStandardItem(mission.plein ? "Fait" : "Non fait"));
        if (ui->filtreRemboursementLavageCheckBox->isChecked()) row.append(new QStandardItem(mission.remboursementLavage ? "Fait" : "Non fait"));
        if (ui->filtreRemboursementPleinCheckBox->isChecked()) row.append(new QStandardItem(mission.remboursementPlein ? "Fait" : "Non fait"));

        model->appendRow(row);
    }

    ui->missionsTableView->setModel(model);
    ui->missionsTableView->resizeColumnsToContents();
    ui->missionsTableView->resizeRowsToContents();
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
            mission.remboursementLavage = ui->remboursementLavageCheckBox->isChecked();
            mission.remboursementPlein = ui->remboursementPleinCheckBox->isChecked();

            // Mettre à jour la base de données
            QSqlQuery query;
            query.prepare("UPDATE missions SET remboursementLavage = :remboursementLavage, remboursementPlein = :remboursementPlein WHERE immatriculation = :immatriculation");
            query.bindValue(":remboursementLavage", mission.remboursementLavage);
            query.bindValue(":remboursementPlein", mission.remboursementPlein);
            query.bindValue(":immatriculation", mission.immatriculation);
            query.exec();

            loadMissions();
        }
    }
    else
    {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner une mission à modifier");
    }
}
