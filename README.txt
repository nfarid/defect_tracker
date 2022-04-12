
A very simple defect tracker.

Website: https://simple-defect-tracker.herokuapp.com



This application requires a postgres database.

Before running the application tools/create_tables.sql should be used to create tables in the database.
tools/create_test_data.sql can be run for example data for the application, (and should be run before any tests).

The database url must be in the form:
scheme://username:password@host:port/path





This application uses a docker container, to create the image:
docker build . -t defect_tracker


To run the container (linux):
docker run -d --network="host" -e DATABASE_URL="dbUrl" -e PORT="portNum" defect_tracker
[replace porNum with the port]
[dbUrl is the a datbase url in the form: scheme://username:password@host:port/path]

To run the container (windows):
docker run -dp portNum:3000 -e DATABASE_URL="dbUrl" defect_tracker
[replace porNum with the port]
[replace dbUrl with the the datbase url in the form: scheme://username:password@host:port/path (replace localhost with host.docker.internal)]


To stop the container:
docker stop containerId

