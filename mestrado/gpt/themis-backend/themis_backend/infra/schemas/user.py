import uuid
from datetime import datetime

from sqlalchemy import CHAR, VARCHAR, DateTime, Uuid, func
from sqlalchemy.orm import MappedColumn, mapped_column

from themis_backend.infra.schemas.base import BaseSchema


class UserSchema(BaseSchema):
    __tablename__ = 'users'

    id: MappedColumn[uuid.UUID] = mapped_column(
        Uuid, primary_key=True, default=uuid.uuid4
    )
    name: MappedColumn[str] = mapped_column(VARCHAR(length=80))
    email: MappedColumn[str] = mapped_column(VARCHAR(length=255), unique=True)
    hashed_password: MappedColumn[str] = mapped_column(CHAR(length=60))
    created_at: MappedColumn[datetime] = mapped_column(
        DateTime(timezone=True), default=func.now()
    )
    updated_at: MappedColumn[datetime] = mapped_column(
        DateTime(timezone=True), default=func.now()
    )

    def __repr__(self):
        return f'<UserSchema (Name={self.name}, E-mail={self.email}>'
