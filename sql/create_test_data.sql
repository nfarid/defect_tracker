
--For creating some test data.

--First delete any data already in the database
DELETE FROM Account;
DELETE FROM Project;
DELETE FROM Ticket;
DELETE FROM Staff;
DELETE FROM Comment;


-- Create the example data
INSERT INTO account(username, password_hash) VALUES
    ('username', '$argon2id$v=19$m=262144,t=3,p=1$6StGe1EqVL4n7g8cKNtl3g$zt2IXeni0yr2mF97dyHAWHJVb4gAs9QOHvEZ3iyoiME'),
    ('example_manager', '$argon2id$v=19$m=262144,t=3,p=1$6StGe1EqVL4n7g8cKNtl3g$zt2IXeni0yr2mF97dyHAWHJVb4gAs9QOHvEZ3iyoiME'),
    ('example_staff_1', '$argon2id$v=19$m=262144,t=3,p=1$6StGe1EqVL4n7g8cKNtl3g$zt2IXeni0yr2mF97dyHAWHJVb4gAs9QOHvEZ3iyoiME'),
    ('example_staff_2', '$argon2id$v=19$m=262144,t=3,p=1$6StGe1EqVL4n7g8cKNtl3g$zt2IXeni0yr2mF97dyHAWHJVb4gAs9QOHvEZ3iyoiME'),
    ('example_staff_3', '$argon2id$v=19$m=262144,t=3,p=1$6StGe1EqVL4n7g8cKNtl3g$zt2IXeni0yr2mF97dyHAWHJVb4gAs9QOHvEZ3iyoiME'),
    ('example_user', '$argon2id$v=19$m=262144,t=3,p=1$6StGe1EqVL4n7g8cKNtl3g$zt2IXeni0yr2mF97dyHAWHJVb4gAs9QOHvEZ3iyoiME'),
    ('demo_regular_user', '$argon2id$v=19$m=262144,t=3,p=1$6StGe1EqVL4n7g8cKNtl3g$zt2IXeni0yr2mF97dyHAWHJVb4gAs9QOHvEZ3iyoiME'),
    ('demo_project_staff', '$argon2id$v=19$m=262144,t=3,p=1$6StGe1EqVL4n7g8cKNtl3g$zt2IXeni0yr2mF97dyHAWHJVb4gAs9QOHvEZ3iyoiME'),
    ('demo_project_manager', '$argon2id$v=19$m=262144,t=3,p=1$6StGe1EqVL4n7g8cKNtl3g$zt2IXeni0yr2mF97dyHAWHJVb4gAs9QOHvEZ3iyoiME')
;

INSERT INTO project(title, description, manager_id) VALUES
    ('project', 'description', (SELECT id FROM account WHERE username='username') ),
    ('example_project_1', 'This is an example description.', (SELECT id FROM account WHERE username='example_manager') ),
    ('example_project_2', 'Another example description!', (SELECT id FROM account WHERE username='example_manager') ),
    ('demo_project', 'This is a demo project.', (SELECT id FROM account WHERE username='demo_project_manager') )
;

INSERT INTO ticket(title, description, status, severity, created_date, resolved_date, assigned_id, project_id, reporter_id) VALUES
    ('ticket', 'description', 'new', 'low', CURRENT_TIMESTAMP, NULL, NULL, (SELECT id FROM project WHERE title='project'), (SELECT id FROM account WHERE username='example_user') ),
    ('example_ticket_1', 'example_description', 'new', 'low', CURRENT_TIMESTAMP, NULL, NULL, (SELECT id FROM project WHERE title='example_project_1'), (SELECT id FROM account WHERE username='example_user') ),
    ('example_ticket_2', 'another_description', 'confirmed', 'high', CURRENT_TIMESTAMP, NULL, (SELECT id FROM account WHERE username='example_staff_1'), (SELECT id FROM project WHERE title='example_project_1'), (SELECT id FROM account WHERE username='example_user') ),
    ('demo_ticket_1', 'this is the first demo ticket', 'new', 'high', CURRENT_TIMESTAMP, NULL, NULL, (SELECT id FROM project WHERE title='demo_project'), (SELECT id FROM account WHERE username='demo_regular_user') ),
    ('demo_ticket_2', 'this is the second demo ticket', 'confirmed', 'medium', CURRENT_TIMESTAMP, NULL, (SELECT id FROM account WHERE username='demo_project_staff'), (SELECT id FROM project WHERE title='demo_project'), (SELECT id FROM account WHERE username='demo_regular_user') ),
    ('demo_ticket_3', 'this is the third demo ticket', 'confirmed', 'high', CURRENT_TIMESTAMP, CURRENT_TIMESTAMP, NULL, (SELECT id FROM project WHERE title='demo_project'), (SELECT id FROM account WHERE username='demo_regular_user') )
;


INSERT INTO staff(project_id, staff_id) VALUES
    ( (SELECT id FROM project WHERE title='example_project_1'), (SELECT id FROM account WHERE username='example_staff_1') ),
    ( (SELECT id FROM project WHERE title='example_project_1'), (SELECT id FROM account WHERE username='example_staff_2') ),
    ( (SELECT id FROM project WHERE title='example_project_1'), (SELECT id FROM account WHERE username='example_staff_3') ),
    ( (SELECT id FROM project WHERE title='example_project_2'), (SELECT id FROM account WHERE username='example_staff_1') ),
    ( (SELECT id FROM project WHERE title='demo_project'), (SELECT id FROM account WHERE username='demo_project_staff') )
;

INSERT INTO comment(post, created_date, ticket_id, poster_id) VALUES
    ('This is an example post', CURRENT_TIMESTAMP, (SELECT id FROM ticket WHERE title='example_ticket_1'), (SELECT id FROM account WHERE username='username') ),
    ('Confirmed', CURRENT_TIMESTAMP, (SELECT id FROM ticket WHERE title='demo_ticket_2'), (SELECT id FROM account WHERE username='demo_project_manager') ),
    ('Resolved', CURRENT_TIMESTAMP, (SELECT id FROM ticket WHERE title='demo_ticket_3'), (SELECT id FROM account WHERE username='demo_project_staff') ),
    ('This is resolved', CURRENT_TIMESTAMP, (SELECT id FROM ticket WHERE title='demo_ticket_3'), (SELECT id FROM account WHERE username='demo_project_manager') )
;
