
A very simple defect tracker.

Website: https://simple-defect-tracker.herokuapp.com



This application requires a postgres database.

Before running the application tools/create_tables.sql should be used to create tables in the database.
tools/create_test_data.sql can be run for example data for the application, (and should be run before any tests).

The database url must be in the form:
scheme://username:password@host:port/path





This application uses a docker container, to create the image:
docker build . -t defect_tracker


To run the container:
docker run -d --network="host" -e DATABASE_URL="dbUrl" -e PORT="portNum" defect_tracker
[porNum is a number from 0 to 65535]
[dbUrl is the a datbase url in the form: scheme://username:password@host:port/path]

To stop the container:
docker stop -a
[Or docker stop -a [containerId] ]

