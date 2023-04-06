
A very simple defect tracker.



This application requires a postgres database.

Before running the application tools/create_tables.sql should be used to create tables in the database.
tools/create_test_data.sql can be run for example data for the application, (and should be run before any tests).

The database url must be in the form:
scheme://username:password@host:port/path
• scheme – the application will assume a scheme of “postgres”, so this is not required
• username – the username that is used to connect to the database
• password – the password that is used to connect to the database
• host – the address of the database, if it’s local it would be “localhost” or “host.docker.internal”
• port – would be 5432
• path – the name of the database





This application uses a docker container, to create the image:
docker build . -t defect_tracker


The command to run the container in Linux is the following:
docker run -d --network="host" -e DATABASE_URL="[enter_database_url_here]" defect_tracker

The command to run the container on Windows is the following:
docker run -dp 3000:3000 -e DATABASE_URL="[enter_database_url_here]" defect_tracker

[enter_database_url_here] would need to be replaced with the database url



To stop the container:
docker stop containerId

