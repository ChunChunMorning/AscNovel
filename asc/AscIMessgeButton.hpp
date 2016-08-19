# pragma once

namespace asc
{
	class IMessageButton
	{
	public:

		virtual ~IMessageButton() = default;

		virtual void init() = 0;

		virtual void update() = 0;

		virtual void onClick() = 0;

		virtual void draw() const = 0;

	};
}