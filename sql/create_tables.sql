
-- Remove everything from the database
DROP SCHEMA IF EXISTS public CASCADE;
CREATE SCHEMA public;


-- Create the tables
CREATE TABLE account(
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    username TEXT NOT NULL UNIQUE,
    password_hash TEXT NOT NULL
);

CREATE TABLE project(
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    title TEXT NOT NULL UNIQUE,
    description TEXT NOT NULL,
    manager_id INTEGER NOT NULL REFERENCES account(id) ON DELETE CASCADE
);

CREATE TYPE status_type AS ENUM('new', 'confirmed', 'unreproducible', 'resolved', 'reopened');
CREATE TYPE severity_type AS ENUM ('low', 'medium', 'high');

CREATE TABLE ticket(
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    title TEXT NOT NULL,
    description TEXT NOT NULL,
    status status_type NOT NULL,
    severity severity_type NOT NULL,
    created_date TIMESTAMP NOT NULL,
    resolved_date TIMESTAMP,
    image_filename TEXT,
    reporter_id INTEGER NOT NULL REFERENCES account(id) ON DELETE CASCADE,
    assigned_id INTEGER REFERENCES account(id) ON DELETE SET NULL,
    project_id INTEGER NOT NULL REFERENCES project(id) ON DELETE CASCADE
);

CREATE TABLE staff(
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    project_id INTEGER NOT NULL REFERENCES project(id) ON DELETE CASCADE,
    staff_id INTEGER NOT NULL REFERENCES account(id) ON DELETE CASCADE
);

CREATE TABLE comment(
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    post TEXT NOT NULL,
    created_date TIMESTAMP NOT NULL,
    ticket_id INTEGER NOT NULL REFERENCES ticket(id) ON DELETE CASCADE,
    poster_id INTEGER NOT NULL REFERENCES account(id) ON DELETE CASCADE
);

CREATE TABLE notification(
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    summary TEXT NOT NULL,
    user_id INTEGER NOT NULL REFERENCES account(id) ON DELETE CASCADE,
    ticket_id INTEGER NOT NULL REFERENCES ticket(id) ON DELETE CASCADE
);
