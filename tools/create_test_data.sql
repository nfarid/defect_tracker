
--For creating some test data.

--First delete any data already in the database
DELETE FROM Account;
DELETE FROM Project;
DELETE FROM Ticket;
DELETE FROM Staff;
DELETE FROM Comment;


--Then add the test data
INSERT INTO Account(username, password_hash) VALUES
    ('username', '$argon2id$v=19$m=262144,t=3,p=1$6StGe1EqVL4n7g8cKNtl3g$zt2IXeni0yr2mF97dyHAWHJVb4gAs9QOHvEZ3iyoiME'),
    ('example_manager', '$argon2id$v=19$m=262144,t=3,p=1$6StGe1EqVL4n7g8cKNtl3g$zt2IXeni0yr2mF97dyHAWHJVb4gAs9QOHvEZ3iyoiME'),
    ('example_staff_1', '$argon2id$v=19$m=262144,t=3,p=1$6StGe1EqVL4n7g8cKNtl3g$zt2IXeni0yr2mF97dyHAWHJVb4gAs9QOHvEZ3iyoiME'),
    ('example_staff_2', '$argon2id$v=19$m=262144,t=3,p=1$6StGe1EqVL4n7g8cKNtl3g$zt2IXeni0yr2mF97dyHAWHJVb4gAs9QOHvEZ3iyoiME'),
    ('example_staff_3', '$argon2id$v=19$m=262144,t=3,p=1$6StGe1EqVL4n7g8cKNtl3g$zt2IXeni0yr2mF97dyHAWHJVb4gAs9QOHvEZ3iyoiME'),
    ('example_user', '$argon2id$v=19$m=262144,t=3,p=1$6StGe1EqVL4n7g8cKNtl3g$zt2IXeni0yr2mF97dyHAWHJVb4gAs9QOHvEZ3iyoiME');

INSERT INTO Project(project_name, manager_id) VALUES
    ('project', (SELECT id FROM Account WHERE username='username') ),
    ('example_project_1', (SELECT id FROM Account WHERE username='example_manager') ),
    ('example_project_2', (SELECT id FROM Account WHERE username='example_manager') );

INSERT INTO Ticket(title, descr, ticket_status, severity, created_date, assigned, project) VALUES
    ('ticket', 'description', 'new', 'low', CURRENT_TIMESTAMP, NULL, (SELECT id FROM Project WHERE project_name='project') ),
    ('example_ticket_1', 'example_description', 'new', 'low', CURRENT_TIMESTAMP, NULL, (SELECT id FROM Project WHERE project_name='example_project_1') ),
    ('example_ticket_2', 'another_description', 'confirmed', 'high', CURRENT_TIMESTAMP, (SELECT id FROM Account WHERE username='example_staff_1'), (SELECT id FROM Project WHERE project_name='example_project_1') );


INSERT INTO Staff(project_id, staff_id) VALUES
    ( (SELECT id FROM Project WHERE project_name='example_project_1'), (SELECT id FROM Account WHERE username='example_staff_1') );

INSERT INTO Comment(post, created_date, project) VALUES
    ('This is an example post', CURRENT_TIMESTAMP, (SELECT id FROM Ticket WHERE title='example_ticket_1') );
