CREATE TABLE IF NOT EXISTS user_profile (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  first_name TEXT NOT NULL,
  last_name TEXT NOT NULL,
  email TEXT UNIQUE NOT NULL,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS user_account (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  username TEXT UNIQUE NOT NULL,
  password TEXT NOT NULL,
  profile_id INTEGER,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (profile_id) REFERENCES user_profile(id)
);
`
INSERT INTO user_profile (first_name, last_name, email)
VALUES ('Alice', 'Nguyen', 'alice@example.com');`

INSERT INTO user_account (username, password, profile_id)
VALUES ('alice123', 'securepass', 1);

SELECT *
FROM user_account
WHERE profile_id = 1;

UPDATE user_profile
SET first_name = 'Alicia', email = 'alicia@example.com'
WHERE id = 1;

UPDATE user_account
SET password = 'newsecurepassword'
WHERE username = 'alice123';


DELETE FROM user_profile WHERE id = 1;
DELETE FROM user_account WHERE username = 'alice123';