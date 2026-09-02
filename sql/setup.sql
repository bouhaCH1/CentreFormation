-- ============================================================
-- Oracle XE Setup Script - Centre de Formation
-- Run as: SYSTEM / password: 180702
-- ============================================================

-- Drop existing tables (safe cleanup)
BEGIN EXECUTE IMMEDIATE 'DROP TABLE COURS CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TABLE SALLE CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/

-- Drop sequences
BEGIN EXECUTE IMMEDIATE 'DROP SEQUENCE SEQ_SALLE'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP SEQUENCE SEQ_COURS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/

-- ============================================================
-- TABLE: SALLE
-- ============================================================
CREATE TABLE SALLE (
    ID_SALLE    NUMBER          CONSTRAINT PK_SALLE PRIMARY KEY,
    NOM_SALLE   VARCHAR2(100)   CONSTRAINT NN_NOM_SALLE  NOT NULL,
    CAPACITE    NUMBER          CONSTRAINT NN_CAP        NOT NULL,
    TYPE_SALLE  VARCHAR2(50)    CONSTRAINT NN_TYPE       NOT NULL,
    EQUIPEMENT  VARCHAR2(200),
    DISPONIBLE  NUMBER(1)       DEFAULT 1 NOT NULL,
    CONSTRAINT CK_CAPACITE   CHECK (CAPACITE > 0),
    CONSTRAINT CK_DISPONIBLE CHECK (DISPONIBLE IN (0, 1)),
    CONSTRAINT CK_TYPE_SALLE CHECK (TYPE_SALLE IN ('Cours', 'TP', 'Amphitheatre', 'Conference'))
);

-- ============================================================
-- TABLE: COURS
-- ============================================================
CREATE TABLE COURS (
    ID_COURS    NUMBER          CONSTRAINT PK_COURS PRIMARY KEY,
    NOM_COURS   VARCHAR2(150)   CONSTRAINT NN_NOM_COURS NOT NULL,
    DESCRIPTION VARCHAR2(500),
    DUREE       NUMBER          CONSTRAINT NN_DUREE NOT NULL,
    NIVEAU      VARCHAR2(50)    CONSTRAINT NN_NIVEAU NOT NULL,
    CATEGORIE   VARCHAR2(100)   CONSTRAINT NN_CATEGORIE NOT NULL,
    DATE_DEBUT  DATE            CONSTRAINT NN_DATE_DEBUT NOT NULL,
    DATE_FIN    DATE            CONSTRAINT NN_DATE_FIN NOT NULL,
    ID_SALLE    NUMBER          CONSTRAINT FK_COURS_SALLE REFERENCES SALLE(ID_SALLE),
    CONSTRAINT CK_DUREE   CHECK (DUREE > 0),
    CONSTRAINT CK_NIVEAU  CHECK (NIVEAU IN ('Debutant', 'Intermediaire', 'Avance')),
    CONSTRAINT CK_DATES   CHECK (DATE_FIN >= DATE_DEBUT)
);

-- ============================================================
-- SEQUENCES (auto-increment)
-- ============================================================
CREATE SEQUENCE SEQ_SALLE START WITH 1 INCREMENT BY 1 NOCACHE NOCYCLE;
CREATE SEQUENCE SEQ_COURS START WITH 1 INCREMENT BY 1 NOCACHE NOCYCLE;

-- ============================================================
-- TRIGGERS (auto PK)
-- ============================================================
CREATE OR REPLACE TRIGGER TRG_SALLE
BEFORE INSERT ON SALLE
FOR EACH ROW
BEGIN
    IF :NEW.ID_SALLE IS NULL THEN
        SELECT SEQ_SALLE.NEXTVAL INTO :NEW.ID_SALLE FROM DUAL;
    END IF;
END;
/

CREATE OR REPLACE TRIGGER TRG_COURS
BEFORE INSERT ON COURS
FOR EACH ROW
BEGIN
    IF :NEW.ID_COURS IS NULL THEN
        SELECT SEQ_COURS.NEXTVAL INTO :NEW.ID_COURS FROM DUAL;
    END IF;
END;
/

-- ============================================================
-- SAMPLE DATA - SALLES
-- ============================================================
INSERT INTO SALLE (NOM_SALLE, CAPACITE, TYPE_SALLE, EQUIPEMENT, DISPONIBLE)
VALUES ('Salle A101', 30, 'Cours', 'Tableau blanc, Projecteur', 1);

INSERT INTO SALLE (NOM_SALLE, CAPACITE, TYPE_SALLE, EQUIPEMENT, DISPONIBLE)
VALUES ('Amphitheatre Principal', 200, 'Amphitheatre', 'Sono, Projecteur HD, Micros', 1);

INSERT INTO SALLE (NOM_SALLE, CAPACITE, TYPE_SALLE, EQUIPEMENT, DISPONIBLE)
VALUES ('TP Informatique 1', 25, 'TP', 'Ordinateurs, Reseau, Imprimante', 1);

INSERT INTO SALLE (NOM_SALLE, CAPACITE, TYPE_SALLE, EQUIPEMENT, DISPONIBLE)
VALUES ('Salle B205', 40, 'Cours', 'Tableau noir, Climatisation', 0);

INSERT INTO SALLE (NOM_SALLE, CAPACITE, TYPE_SALLE, EQUIPEMENT, DISPONIBLE)
VALUES ('TP Electronique', 20, 'TP', 'Oscilloscopes, Multimetres, Alimentations', 1);

INSERT INTO SALLE (NOM_SALLE, CAPACITE, TYPE_SALLE, EQUIPEMENT, DISPONIBLE)
VALUES ('Salle Conference C1', 15, 'Conference', 'Table ronde, Ecran TV, Videoconference', 1);

-- ============================================================
-- SAMPLE DATA - COURS
-- ============================================================
INSERT INTO COURS (NOM_COURS, DESCRIPTION, DUREE, NIVEAU, CATEGORIE, DATE_DEBUT, DATE_FIN, ID_SALLE)
VALUES ('Introduction au C++', 'Bases du langage C++: variables, fonctions, pointeurs', 40,
        'Debutant', 'Informatique', TO_DATE('2026-09-01', 'YYYY-MM-DD'), TO_DATE('2026-09-30', 'YYYY-MM-DD'), 1);

INSERT INTO COURS (NOM_COURS, DESCRIPTION, DUREE, NIVEAU, CATEGORIE, DATE_DEBUT, DATE_FIN, ID_SALLE)
VALUES ('Qt Framework Avance', 'Developpement GUI professionnel avec Qt6', 60,
        'Intermediaire', 'Informatique', TO_DATE('2026-10-01', 'YYYY-MM-DD'), TO_DATE('2026-11-15', 'YYYY-MM-DD'), 3);

INSERT INTO COURS (NOM_COURS, DESCRIPTION, DUREE, NIVEAU, CATEGORIE, DATE_DEBUT, DATE_FIN, ID_SALLE)
VALUES ('Algorithmique et Structures de donnees', 'Algorithmes de tri et recherche', 50,
        'Avance', 'Informatique', TO_DATE('2026-09-15', 'YYYY-MM-DD'), TO_DATE('2026-10-31', 'YYYY-MM-DD'), 2);

INSERT INTO COURS (NOM_COURS, DESCRIPTION, DUREE, NIVEAU, CATEGORIE, DATE_DEBUT, DATE_FIN, ID_SALLE)
VALUES ('Base de donnees Oracle', 'SQL, PL/SQL et administration Oracle', 35,
        'Intermediaire', 'Base de donnees', TO_DATE('2026-09-01', 'YYYY-MM-DD'), TO_DATE('2026-09-25', 'YYYY-MM-DD'), 3);

INSERT INTO COURS (NOM_COURS, DESCRIPTION, DUREE, NIVEAU, CATEGORIE, DATE_DEBUT, DATE_FIN, ID_SALLE)
VALUES ('Reseaux Informatiques', 'Protocoles TCP/IP et architecture reseau', 45,
        'Debutant', 'Reseaux', TO_DATE('2026-10-01', 'YYYY-MM-DD'), TO_DATE('2026-11-01', 'YYYY-MM-DD'), 1);

INSERT INTO COURS (NOM_COURS, DESCRIPTION, DUREE, NIVEAU, CATEGORIE, DATE_DEBUT, DATE_FIN, ID_SALLE)
VALUES ('Intelligence Artificielle', 'Introduction au Machine Learning', 70,
        'Avance', 'Informatique', TO_DATE('2026-11-01', 'YYYY-MM-DD'), TO_DATE('2026-12-20', 'YYYY-MM-DD'), 2);

INSERT INTO COURS (NOM_COURS, DESCRIPTION, DUREE, NIVEAU, CATEGORIE, DATE_DEBUT, DATE_FIN, ID_SALLE)
VALUES ('Securite Informatique', 'Cryptographie et securite des systemes', 40,
        'Intermediaire', 'Securite', TO_DATE('2026-09-20', 'YYYY-MM-DD'), TO_DATE('2026-10-20', 'YYYY-MM-DD'), 1);

COMMIT;

-- Verify
SELECT 'SALLES: ' || COUNT(*) FROM SALLE;
SELECT 'COURS: ' || COUNT(*) FROM COURS;

EXIT;

