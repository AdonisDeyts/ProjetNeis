#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QDateTimeEdit>
#include <QtSql/QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Vehicule
{
    QString immatriculation;
    QString description;
    QString couleur;
    QString latitude;
    QString longitude;
    double kilometrage;
    bool disponible;
};

struct Conducteur
{
    QString nom;
    QString email;
    QString prenom;
};

struct Client
{
    QString adresseChantier;
    QString nom;
};

struct Mission
{
    QString immatriculation;
    QString deltaDistance;
    QDateTime horodatageDepart;
    QDateTime horodatageArrivee;
    bool lavage;
    bool plein;
    bool remboursementPlein;
    bool remboursementLavage;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadVehicules();
    void loadConducteurs();
    void loadClients();
    void loadMissions();
    void on_editMissionButton_clicked();

private:
    Ui::MainWindow *ui;
    QList<Vehicule> vehicules;
    QList<Conducteur> conducteurs;
    QList<Client> clients;
    QList<Mission> missions;
    bool createConnection(); // Méthode pour établir la connexion à la base de données
};

#endif // MAINWINDOW_H
