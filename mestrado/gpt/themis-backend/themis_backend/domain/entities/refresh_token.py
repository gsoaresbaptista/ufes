from datetime import datetime
from typing import Any, NamedTuple
from uuid import UUID


class RefreshToken(NamedTuple):
    id: UUID
    user_id: UUID
    expires_in: datetime

    def to_dict(self) -> dict[str, Any]:
        return {
            'id': self.id.hex,
            'user_id': self.user_id.hex,
            'expires_in': self.expires_in.strftime('%Y-%m-%d, %H:%M:%S'),
        }
