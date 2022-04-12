
A very simple defect tracker.

Website: https://simple-defect-tracker.herokuapp.com



This application requires a postgres database.

Before running the application tools/create_tables.sql should be used to create tables in the database.
tools/create_test_data.sql can be run for example data for the application, (and should be run before any tests).

The database url must be in the form:
scheme://username:password@host:port/path

The database can be passed through the command line.
defect_tracker --db [database url goes here]

Alternatively the DATABASE_URL can be set, and the application will use that:
export DATABASE_URL=[datebase url goes here]




The port this application listen to can also be specified either by:
Using the the --port argument:
Or the PORT environment variable:




