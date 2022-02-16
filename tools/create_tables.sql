
DROP SCHEMA IF EXISTS public CASCADE;
CREATE SCHEMA public;

CREATE TABLE Account(
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    username TEXT NOT NULL UNIQUE,
    password_digest TEXT NOT NULL
);

CREATE TABLE Project(
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    project_name TEXT NOT NULL UNIQUE,
    manager_id INTEGER REFERENCES Account(id) ON DELETE CASCADE
);

CREATE TYPE StatusType AS ENUM('new', 'confirmed', 'unreproducible', 'assigned', 'resolved', 'reopened');
CREATE TYPE SeverityType AS ENUM ('low', 'medium', 'high');

CREATE TABLE Ticket(
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    title TEXT NOT NULL,
    descr TEXT NOT NULL,
    project_module TEXT,
    ticket_status StatusType NOT NULL,
    severity SeverityType NOT NULL,
    assigned INTEGER REFERENCES Account(id) ON DELETE SET NULL,
    project INTEGER REFERENCES Project(id) ON DELETE CASCADE
);

CREATE TABLE Staff(
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    project_id INTEGER REFERENCES Ticket(id) ON DELETE CASCADE,
    staff_id INTEGER REFERENCES Ticket(id) ON DELETE CASCADE
);

CREATE TABLE Comment(
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    text TEXT NOT NULL
);
