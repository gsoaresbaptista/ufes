from dotenv import dotenv_values

env_file = dotenv_values('.env')


class DatabaseSettings:
    DATABASE_USER: str = env_file.get('DATABASE_USER')
    DATABASE_PASSWORD: str = env_file.get('DATABASE_PASSWORD')
    DATABASE_HOST: str = env_file.get('DATABASE_HOST')
    DATABASE_PORT: int = int(env_file.get('DATABASE_PORT'))
    DATABASE_NAME: str = env_file.get('DATABASE_NAME')


class TokenSettings:
    ALGORITHM: str = env_file.get('ACCESS_TOKEN_ALGORITHM')
    SECRET_KEY: str = env_file.get('ACCESS_TOKEN_SECRET_KEY')
    EXPIRE_MINUTES: int = int(env_file.get('ACCESS_TOKEN_EXPIRE_MINUTES'))
    REFRESH_TOKEN_EXPIRE_MINUTES: int = int(
        env_file.get('REFRESH_TOKEN_EXPIRE_MINUTES')
    )


class ModelSettings:
    FOLDER_PATH: str = env_file.get('MODEL_FOLDER_PATH')
    FILE_PATH: str = env_file.get('MODEL_FILE_PATH')
    TYPE: str = env_file.get('MODEL_TYPE')
    GPU_LAYERS: int = int(env_file.get('MODEL_GPU_LAYERS', 50))
    MAX_NEW_TOKENS: int = int(env_file.get('MODEL_MAX_NEW_TOKENS'))
    CONTEXT_LENGTH: int = int(env_file.get('MODEL_CONTEXT_LENGTH'))
