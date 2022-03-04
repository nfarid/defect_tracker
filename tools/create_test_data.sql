
--For creating some test data.

--First delete any data already in the database
DELETE FROM Account;
DELETE FROM Project;
DELETE FROM Ticket;
DELETE FROM Staff;
DELETE FROM Comment;


--Then add the test data
INSERT INTO Account(username, password_digest) VALUES
    ('username', '$argon2id$v=19$m=262144,t=3,p=1$6StGe1EqVL4n7g8cKNtl3g$zt2IXeni0yr2mF97dyHAWHJVb4gAs9QOHvEZ3iyoiME');

INSERT INTO Project(project_name, manager_id) VALUES
    ('project', (SELECT id FROM Account WHERE username='username') );

INSERT INTO Ticket(title, descr, project_module, ticket_status, severity, assigned, project) VALUES
    ('ticket', 'description', 'mod', 'new', 'low', NULL, (SELECT id FROM Project WHERE project_name='project') );
