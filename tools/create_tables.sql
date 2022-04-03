
DROP SCHEMA IF EXISTS public CASCADE;
CREATE SCHEMA public;

CREATE TABLE Account(
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    username TEXT NOT NULL UNIQUE,
    password_hash TEXT NOT NULL
);

CREATE TABLE Project(
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    title TEXT NOT NULL UNIQUE,
    description TEXT NOT NULL,
    manager_id INTEGER REFERENCES Account(id) ON DELETE CASCADE
);

CREATE TYPE StatusType AS ENUM('new', 'confirmed', 'unreproducible', 'resolved', 'reopened');
CREATE TYPE SeverityType AS ENUM ('low', 'medium', 'high');

CREATE TABLE Ticket(
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    title TEXT NOT NULL,
    description TEXT NOT NULL,
    status StatusType NOT NULL,
    severity SeverityType NOT NULL,
    created_date TIMESTAMP NOT NULL,
    resolved_date TIMESTAMP,
    image_filename TEXT,
    reporter_id INTEGER NOT NULL REFERENCES Account(id) ON DELETE CASCADE,
    assigned_id INTEGER REFERENCES Account(id) ON DELETE SET NULL,
    project_id INTEGER NOT NULL REFERENCES Project(id) ON DELETE CASCADE
);

CREATE TABLE Staff(
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    project_id INTEGER NOT NULL REFERENCES Project(id) ON DELETE CASCADE,
    staff_id INTEGER NOT NULL REFERENCES Account(id) ON DELETE CASCADE
);

CREATE TABLE Comment(
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    post TEXT NOT NULL,
    created_date TIMESTAMP NOT NULL,
    ticket_id INTEGER NOT NULL REFERENCES Ticket(id) ON DELETE CASCADE,
    poster_id INTEGER NOT NULL REFERENCES Account(id) ON DELETE CASCADE
);

CREATE TABLE Notification(
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    summary TEXT NOT NULL,
    user_id INTEGER NOT NULL REFERENCES Account(id) ON DELETE CASCADE,
    ticket_id INTEGER NOT NULL REFERENCES Ticket(id) ON DELETE CASCADE
);
