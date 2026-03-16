# Minibricks

**Minibricks** is a small, SQLite-first C++ platform for user data + tiny machine learning workflows.

Think of it as a **mini data**, but aimed at:

- local development,
- embedded / desktop workloads,
- small AI or ML models,
- fast iteration without standing up a distributed stack.

It gives you a clean starting point for building:

- a **user data store**,
- a **feature store**,
- an **experiment tracker**,
- a **model registry**,
- and a **fast local inference path**.


## Architecture

```text
                 +---------------------------+
                 |       CLI / Service       |
                 | init / seed / train       |
                 | predict / list-models     |
                 +-------------+-------------+
                               |
                               v
                 +---------------------------+
                 |      Application Core     |
                 | FeatureStore              |
                 | DatasetBuilder            |
                 | Trainer                   |
                 | ModelRegistry             |
                 +-------------+-------------+
                               |
                               v
                 +---------------------------+
                 |          SQLite           |
                 | users                     |
                 | user_events               |
                 | user_features             |
                 | experiments               |
                 | models                    |
                 | predictions               |
                 +---------------------------+
```

---