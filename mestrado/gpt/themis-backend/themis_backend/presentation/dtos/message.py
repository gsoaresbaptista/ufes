from typing import NamedTuple
from uuid import UUID


class CreateMessageDTO(NamedTuple):
    user_id: UUID | str
    question: str
    answer: str


class DeleteMessageDTO(NamedTuple):
    user_id: UUID | str
    message_id: UUID | str


class ClearChatDTO(NamedTuple):
    user_id: UUID | str


class ContinueAnswerDTO(NamedTuple):
    message_id: UUID | str
    user_id: UUID | str


class UpdateMessageAnswerDTO(NamedTuple):
    user_id: UUID | str
    message_id: UUID | str
    answer: str
