from typing import Optional
from uuid import UUID

from sqlalchemy import delete, desc, select, update, func

from themis_backend.domain.entities import Message
from themis_backend.domain.repositories import MessageRepository
from themis_backend.infra.database import Session
from themis_backend.infra.schemas import MessageSchema


def user_row_to_entity(row: MessageSchema) -> Message:
    return Message(
        id=row.id,
        user_id=row.user_id,
        question=row.question,
        answer=row.answer,
        created_at=row.created_at,
    )


class PostgreMessageRepository(MessageRepository):
    async def create(
        self, user_id: UUID | str, question: str, answer: str
    ) -> Optional[Message]:

        message = MessageSchema(
            user_id=user_id,
            question=question,
            answer=answer,
        )

        async with Session() as session:
            session.add(message)
            await session.commit()
            await session.refresh(message)
        return user_row_to_entity(message)

    async def search_by_user_id(self, user_id: UUID | str) -> list[Message]:
        async with Session() as session:
            query = select(MessageSchema).where(
                MessageSchema.user_id == user_id
            )
            messages = await session.execute(query)
            messages = messages.scalars()

        return [user_row_to_entity(message).to_dict() for message in messages]

    async def get_last_message(self, user_id: UUID | str) -> list[Message]:
        async with Session() as session:
            query = (
                select(MessageSchema)
                .where(MessageSchema.user_id == user_id)
                .order_by(desc(MessageSchema.created_at))
            )
            message = await session.execute(query)
            message = message.scalar()

        return (
            user_row_to_entity(message).to_dict()
            if message is not None
            else None
        )

    async def search_by_id(
        self, message_id: UUID | str, user_id: UUID | str
    ) -> Optional[Message]:
        async with Session() as session:
            if message_id != '' and message_id is not None:
                query = select(MessageSchema).where(
                    (MessageSchema.id == message_id)
                    & (MessageSchema.user_id == user_id)
                )
            else:
                query = (
                    select(MessageSchema)
                    .where(MessageSchema.user_id == user_id)
                    .order_by(MessageSchema.created_at.desc())
                )
            message = await session.execute(query)
            message = message.scalar()

        return user_row_to_entity(message) if message is not None else None

    async def delete(
        self, message_id: UUID | str, user_id: UUID | str
    ) -> UUID | str:
        async with Session() as session:
            query = (
                delete(MessageSchema)
                .where(
                    (MessageSchema.id == message_id)
                    & (MessageSchema.user_id == user_id)
                )
                .returning(MessageSchema.id)
            )
            message = await session.execute(query)
            message = message.fetchone()
            await session.commit()

        return message.hex if message is not None else None

    async def delete_all(self, user_id: UUID | str) -> list[UUID | str]:
        async with Session() as session:
            query = (
                delete(MessageSchema)
                .where((MessageSchema.user_id == user_id))
                .returning(MessageSchema.id)
            )
            messages = await session.execute(query)
            messages = messages.scalars()
            await session.commit()

        return (
            [message.hex for message in messages if messages is not None]
            if messages is not None
            else []
        )

    async def update_answer(
        self, user_id: UUID | str, message_id: UUID | str, answer: str
    ) -> list[UUID | str]:
        async with Session() as session:
            query = (
                update(MessageSchema)
                .where(
                    (MessageSchema.user_id == user_id)
                    & (MessageSchema.id == message_id)
                )
                .values(answer=answer, created_at=func.now())
            )
            await session.execute(query)
            await session.commit()
