from datetime import datetime
from typing import Any, NamedTuple
from uuid import UUID


class User(NamedTuple):
    id: UUID
    name: str
    email: str
    hashed_password: str
    created_at: datetime
    updated_at: datetime

    def to_dict(self, no_password: bool = True) -> dict[str, Any]:
        dict_user = {
            'id': self.id.hex,
            'name': self.name,
            'email': self.email,
            'created_at': self.created_at.strftime('%Y-%m-%d, %H:%M:%S'),
            'updated_at': self.updated_at.strftime('%Y-%m-%d, %H:%M:%S'),
        }

        if not no_password:
            dict_user.update({'hashed_password': self.hashed_password})

        return dict_user
