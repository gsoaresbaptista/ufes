from typing import NamedTuple
from uuid import UUID


class Message(NamedTuple):
    id: UUID | str
    user_id: UUID | str
    question: str
    answer: str
    created_at: str

    def to_dict(self) -> dict[str, str]:
        return {
            'id': self.id.hex if isinstance(self.id, UUID) else self.id,
            'user_id': self.user_id.hex
            if isinstance(self.user_id, UUID)
            else self.user_id,
            'question': self.question,
            'answer': self.answer,
            'created_at': self.created_at.strftime('%Y-%m-%d %H:%M:%S.%f'),
        }
