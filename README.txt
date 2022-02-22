
A defect tracker.

Arguments can be passed through the command line or environment.
E.g. ./defect_tracker --port 5000 --db scheme://userinfo:@host:port/path
Alternatively PORT and DATABASE_URL environment variables can be used instead.
If port is not specified then the port will be 3000. (However database url must be specified).

Use tools/create_tables.sql to create the tables in the database.

Website: https://simple-defect-tracker.herokuapp.com

